/**
 * \file emulator_system.c
 */
#if defined(EMULATOR)

#include <windows.h>
//#include <assert.h>
//#include <crtdbg.h>
#include <SDL/SDL.h>
#include <esic/elcd/lcd.h>
#include <esic/eapi/emulator_system.h>
#include <time.h>
//#include <Winbase.h>

#include <esic/eapi/event.h>


#include <fatfs/ff.h>
#include <diskio_emulator.h>
#include <esic/esic.h>

/* Private variables, instances, etc. */
static FATFS s_fat;

/* Private functions */
static void _createEventFromSDL(PEvent systemEvent, const SDL_Event* psdl_event);
static void _createEventToSDL(PEvent system_event, SDL_Event* psdl_event);
static void _initFileSystem(void);

/* Tmp */
extern void StopTmrThread();

void EmulatorSystemInit(void) {
	int error;
	error = SDL_Init(SDL_INIT_VIDEO);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	//LcdInit(320, 240, s_screen->pixels, 0, DEFAULT_BACKGROUND_COLOR);
	SDL_ShowCursor(0);

	/* Init the file system */
	_initFileSystem();
}



//void EmulatorSystem_addTimer(

/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */

/*
DWORD get_fattime (void)
{
	SYSTEMTIME tm;

	// Get local time
	GetLocalTime(&tm);

	// Pack date and time into a DWORD variable 
	return 	  ((DWORD)(tm.wYear - 1980) << 25)
			| ((DWORD)tm.wMonth << 21)
			| ((DWORD)tm.wDay << 16)
			| (WORD)(tm.wHour << 11)
			| (WORD)(tm.wMinute << 5)
			| (WORD)(tm.wSecond >> 1);
}
*/

DWORD get_fattime(void) {
	struct tm* tm;
	time_t t;

	t = time(NULL);

	tm = localtime(&t);

	return ((DWORD)(tm->tm_year - 10) << 25)		// -10: since 1980?
		|  ((DWORD)(tm->tm_mon << 21)
		|  ((DWORD)(tm->tm_mday) << 16)
		|  (WORD)(tm->tm_hour) << 11)
		|  (WORD)(tm->tm_min << 5)
		|  (WORD)(tm->tm_sec >> 1);

}

void EmulatorSystemDestroy() {
	LcdDestroy();

	diskio_destroy_drive();

	//StopTmrThread();
}

void EmulatorSystemWaitEvent(PEvent esic_event) {
	SDL_Event sdl_event;
	SDL_WaitEvent(&sdl_event);
	_createEventFromSDL(esic_event, &sdl_event);
}

BOOL EmulatorSystemPollEvent(PEvent esic_event) {
	SDL_Event sdl_event;
	BOOL ret = FALSE;
	if(SDL_PollEvent(&sdl_event)) {
		_createEventFromSDL(esic_event, &sdl_event);
		if(esic_event->type != EVENT_NONE) {
			ret = TRUE;
		}
	}
	return ret;
}

void EmulatorSystemDelay(DWORD milliseconds) {
	SDL_Delay(milliseconds);
}

void EmulatorSystemPushEvent(PEvent esic_event) {
	SDL_Event sdl_event;
	_createEventToSDL(esic_event, &sdl_event);
#if defined(_DEBUG)
//	SicPrintfDebug("Pushed %s\n", Event_enumToString(esic_event->type));
#endif
	SDL_PushEvent(&sdl_event);
}

DWORD EmulatorSystemGetTicks(void) {
	return SDL_GetTicks();
}

const BYTE* EmulatorSystemGetKeyboardState() {
	return (BYTE*)SDL_GetKeyState(NULL);
}

static void _createEventFromSDL(PEvent system_event, const SDL_Event* psdl_event) {
	switch(psdl_event->type) {
	case SDL_KEYDOWN:
		system_event->type = EVENT_KEYBOARD_KDOWN;
		system_event->real_event.keyboard_event.state = KEYBOARD_EVENT_RELEASED;
		system_event->real_event.keyboard_event.code = (Keycode)psdl_event->key.keysym.sym;
		/* TODO? */
		break;
	case SDL_KEYUP:
		system_event->type = EVENT_KEYBOARD_KUP;
		system_event->real_event.keyboard_event.state = KEYBOARD_EVENT_PRESSED;
		system_event->real_event.keyboard_event.code = (Keycode)psdl_event->key.keysym.sym;
		break;

	case SDL_USEREVENT:
		system_event->type = (EventType)psdl_event->user.code;
		if(system_event->type == EVENT_WIDGET) {
			system_event->real_event.widget_event.type = (WidgetEventType)psdl_event->user.data1;
			system_event->real_event.widget_event.id   = (WORD)psdl_event->user.data2;
		} else if(system_event->type == EVENT_PAINT) {
			system_event->real_event.widget_event.id   = (WORD)psdl_event->user.data1;
		} else if(system_event->type == EVENT_TIMER) {
			system_event->real_event.timer_event.id   = (WORD)psdl_event->user.data1;
		} else if(system_event->type == EVENT_USER) {
			system_event->real_event.user_event.type  = (DWORD)psdl_event->user.data1;
			system_event->real_event.user_event.param = (DWORD)psdl_event->user.data2;
		}
		break;

	case SDL_QUIT:
		system_event->type = EVENT_QUIT;
		break;
	default:
		system_event->type = EVENT_NONE;
	}
}

static void _createEventToSDL(PEvent system_event, SDL_Event* psdl_event) {
	switch(system_event->type) {
	case EVENT_KEYBOARD_KDOWN:
		psdl_event->type = SDL_KEYDOWN;
		psdl_event->key.keysym.sym = (SDLKey)system_event->real_event.keyboard_event.code;
		psdl_event->key.keysym.mod = KMOD_CAPS;
		/* TODO. */
		break;

	case EVENT_QUIT:
		psdl_event->type = SDL_QUIT;
		break;

	case EVENT_WIDGET:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_WIDGET;
		psdl_event->user.data1 = (void*)system_event->real_event.widget_event.type;
		psdl_event->user.data2 = (void*)system_event->real_event.widget_event.id;
		break;

	case EVENT_PAINT:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_PAINT;
		psdl_event->user.data1 = (void*)system_event->real_event.widget_event.id;
		psdl_event->user.data2 = 0;
		break;

	case EVENT_TIMER:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_TIMER;
		psdl_event->user.data1 = (void*)system_event->real_event.timer_event.id;
		psdl_event->user.data2 = 0;
		break;

	case EVENT_USER:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_USER;
		psdl_event->user.data1 = (void*)system_event->real_event.user_event.type;
		psdl_event->user.data2 = (void*)system_event->real_event.user_event.param;
		break;
	/*
	case EVENT_BLUR:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_BLUR;
		psdl_event->user.data1 = (void*)system_event->real_event.widget_event.id;
		psdl_event->user.data2 = 0;
		break;

	case EVENT_FOCUS:
		psdl_event->type = SDL_USEREVENT;
		psdl_event->user.code = EVENT_FOCUS;
		psdl_event->user.data1 = (void*)system_event->real_event.widget_event.id;
		psdl_event->user.data2 = 0;
		break;
	*/

	}
}

static void _initFileSystem(void) {

	

	FRESULT ret = FR_OK;
	DSTATUS status;

	diskio_assign_drive("fat.bin", SIZE_SECTOR, TOTAL_SIZE/SIZE_SECTOR);

	/* Enforce initialization */
	status = disk_initialize(VOLUME_NUMBER);

	/* Mount the volume */
	ret = f_mount(VOLUME_NUMBER, &s_fat);

	/* Change current drive */
	ret = f_chdrive(VOLUME_NUMBER);
}


#endif /* EMULATOR */