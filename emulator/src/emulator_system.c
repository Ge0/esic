/**
 * \file emulator_system.c
 */
#include <assert.h>
#include <SDL/SDL.h>
#include <esic/elcd/lcd.h>
#include "emulator_system.h"
#include <Winbase.h>

#include <esic/eapi/event.h>

static PAbstractSystem s_singleton_system = NULL;

PAbstractSystem singleton_system() {
	return s_singleton_system;
}


static const vtable_Object s_object_vtable = {
	EmulatorSystem_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_AbstractSystem s_abstract_system_vtable = {
	EmulatorSystem_waitEvent,
	EmulatorSystem_pollEvent,
	EmulatorSystem_update,
	EmulatorSystem_delay,
	EmulatorSystem_getTicks,
	EmulatorSystem_getFrameBuffer,
	EmulatorSystem_enqueueEvent,
	EmulatorSystem_getKeyState
};



/* Private functions */
static void _createEventFromSDL(PEvent systemEvent, const SDL_Event* psdl_event);
static void _initFileSystem(PAbstractSystem self);


PEmulatorSystem EmulatorSystem_constructor(PEmulatorSystem self) {
	int error;

	/* Calling parent constructor */
	self->abstract_system.object.vtable = &s_object_vtable;
	self->abstract_system.vtable        = &s_abstract_system_vtable;
	self->abstract_system.object.size   = sizeof(EmulatorSystem);

	error = SDL_Init(SDL_INIT_VIDEO);
	self->screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	Lcd_init(320, 240, self->abstract_system.vtable->getFrameBuffer(&self->abstract_system), 0, DEFAULT_BACKGROUND_COLOR);
	SDL_ShowCursor(0);

	/* Ensure screen's been created */
	assert(self->screen != NULL);

	/* Init file system? */
	_initFileSystem(&self->abstract_system);

	s_singleton_system = (PAbstractSystem)self;

	return self;
}

static void _initFileSystem(PAbstractSystem self) {
	PEmulatorSystem real_self = (PEmulatorSystem)self;
	FRESULT ret = FR_OK;
	DSTATUS status;

	/* Enforce initialization */
	status = disk_initialize(VOLUME_NUMBER);

	/* Mount the volume */
	ret = f_mount(VOLUME_NUMBER, &real_self->fat);

	/* Change current drive */
	ret = f_chdrive(VOLUME_NUMBER);

}

void EmulatorSystem_destructor(PObject self) {
	Lcd_destroy();
	//StopTmrThread();
}

void EmulatorSystem_waitEvent(PAbstractSystem self, PEvent systemEvent) {
	SDL_Event sdl_event;
	SDL_WaitEvent(&sdl_event);
	_createEventFromSDL(systemEvent, &sdl_event);

}

BOOL EmulatorSystem_pollEvent(PAbstractSystem self, PEvent systemEvent) {
	SDL_Event sdl_event;
	BOOL ret = FALSE;
	if(SDL_PollEvent(&sdl_event)) {
		_createEventFromSDL(systemEvent, &sdl_event);
		if(systemEvent != NULL) {
			ret = TRUE;
		}
	}
	return ret;
}

void EmulatorSystem_update(PAbstractSystem self) {
	PEmulatorSystem real_self = (PEmulatorSystem)self;
	SDL_Flip(real_self->screen);
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
		system_event->real_event.widget_event.id = (WORD)psdl_event->user.data1;
		break;

	case SDL_QUIT:
		system_event->type = EVENT_QUIT;
		break;
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

	}
}

void EmulatorSystem_delay(PAbstractSystem self, DWORD milliseconds) {
	SDL_Delay(milliseconds);
}

DWORD EmulatorSystem_getTicks(PAbstractSystem self) {
	return SDL_GetTicks();
}

void* EmulatorSystem_getFrameBuffer(PAbstractSystem self) {
	return ((PEmulatorSystem)self)->screen->pixels;
}

BYTE* EmulatorSystem_getKeyState(PAbstractSystem self) {
	return (BYTE*)SDL_GetKeyState(NULL);
}

void EmulatorSystem_enqueueEvent(PAbstractSystem self, PEvent system_event) {
	SDL_Event sdl_event;
	_createEventToSDL(system_event, &sdl_event);
	SDL_PushEvent(&sdl_event);
}

//void EmulatorSystem_addTimer(

/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */

DWORD get_fattime (void)
{
	SYSTEMTIME tm;

	/* Get local time */
	GetLocalTime(&tm);

	/* Pack date and time into a DWORD variable */
	return 	  ((DWORD)(tm.wYear - 1980) << 25)
			| ((DWORD)tm.wMonth << 21)
			| ((DWORD)tm.wDay << 16)
			| (WORD)(tm.wHour << 11)
			| (WORD)(tm.wMinute << 5)
			| (WORD)(tm.wSecond >> 1);
}

