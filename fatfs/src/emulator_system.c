#include <assert.h>
#include <SDL/SDL.h>
#include <esic/elcd/lcd.h>
#include "emulator_system.h"
#include <Winbase.h>


extern int assign_drives(void);

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
	EmulatorSystem_getFrameBuffer
};


/* Private functions */
static void _createEvent(PPEvent event, const SDL_Event* psdl_event);
static void _initFileSystem(PAbstractSystem self);


PEmulatorSystem EmulatorSystem_constructor(PEmulatorSystem self) {
	int error;

	/* Calling parent constructor */
	self->abstract_system.object.vtable = &s_object_vtable;
	self->abstract_system.vtable        = &s_abstract_system_vtable;
	self->abstract_system.object.size   = sizeof(EmulatorSystem);

	error = SDL_Init(SDL_INIT_VIDEO);
	self->screen = SDL_SetVideoMode(480, 272, 16, SDL_HWSURFACE);

	/* Ensure screen's been created */
	assert(self->screen != NULL);

	/* Init file system? */
	_initFileSystem(&self->abstract_system);

	return self;
}

static void _initFileSystem(PAbstractSystem self) {
	PEmulatorSystem real_self = (PEmulatorSystem)self;
	FRESULT ret = FR_OK;
	DSTATUS status;
	FIL file;

	/* Specific to Windows layer: drives assignment */
	assign_drives();


	/* Enforce initialization */
	status = disk_initialize(1);

	/* Mount the volume */
	ret = f_mount(1, &real_self->fat);

	/* Change current drive */
	ret = f_chdrive(1);

}

void EmulatorSystem_destructor(PObject self) {


}

void EmulatorSystem_waitEvent(PAstractSystem self, PPEvent systemEvent) {
	SDL_Event sdl_event;
	SDL_WaitEvent(&sdl_event);
	_createEvent(systemEvent, &sdl_event);

}

BOOL EmulatorSystem_pollEvent(PAstractSystem self, PPEvent systemEvent) {
	SDL_Event sdl_event;
	BOOL ret = FALSE;
	if(SDL_PollEvent(&sdl_event)) {
		_createEvent(systemEvent, &sdl_event);
		if(*systemEvent != NULL) {
			ret = TRUE;
		}
	}
	return ret;
}

void EmulatorSystem_update(PAstractSystem self) {
	PEmulatorSystem real_self = (PEmulatorSystem)self;
	SDL_Flip(real_self->screen);
}

static void _createEvent(PPEvent systemEvent, const SDL_Event* psdl_event) {
	switch(psdl_event->type) {
	case SDL_KEYDOWN:
		break;

	case SDL_QUIT:
		*systemEvent = NEW(*systemEvent, Event);
		(*systemEvent)->type = EVENT_QUIT;
		break;
	}
}

void EmulatorSystem_delay(PAstractSystem self, DWORD milliseconds) {
	SDL_Delay(milliseconds);
}

DWORD EmulatorSystem_getTicks(PAstractSystem self) {
	return SDL_GetTicks();
}

void* EmulatorSystem_getFrameBuffer(PAbstractSystem self) {
	return ((PEmulatorSystem)self)->screen->pixels;
}

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
