#ifndef _EMULATOR_SYSTEM_H_
#define _EMULATOR_SYSTEM_H_

#include <esic/eapi/abstract_system.h>
#include <fatfs/diskio.h>
#include <fatfs/ff.h>

#include "SDL\SDL.h"


#define VOLUME_NUMBER	1
#define DEFAULT_BACKGROUND_COLOR RGB_16B(240,240,240)

typedef struct _EmulatorSystem {
	AbstractSystem abstract_system;
	SDL_Surface* screen;
	FATFS fat;
} EmulatorSystem, *PEmulatorSystem;

PEmulatorSystem EmulatorSystem_constructor(PEmulatorSystem self);

/* Virtual functions */
/* Object vtable */
void EmulatorSystem_destructor(PObject self);

/* AbstractSystem vtable */
void	EmulatorSystem_waitEvent(PAbstractSystem, PEvent);
BOOL	EmulatorSystem_pollEvent(PAbstractSystem, PEvent);
void	EmulatorSystem_update(PAbstractSystem);
void	EmulatorSystem_delay(PAbstractSystem, DWORD);
DWORD	EmulatorSystem_getTicks(PAbstractSystem self);
void*	EmulatorSystem_getFrameBuffer(PAbstractSystem self);
void	EmulatorSystem_enqueueEvent(PAbstractSystem self, PEvent system_event);


/* Vtables definition */



#endif /* _EMULATOR_SYSTEM_ */