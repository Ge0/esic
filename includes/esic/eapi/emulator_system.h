/**
 * \file emulator_system.h
 */
#ifndef _EMULATOR_SYSTEM_H_
#define _EMULATOR_SYSTEM_H_

#include <esic/eapi/event.h>
#include <fatfs/diskio.h>
#include <fatfs/ff.h>

#include <SDL/SDL.h>


#define VOLUME_NUMBER	1
#define DEFAULT_BACKGROUND_COLOR RGB_16B(240,240,240)

void EmulatorSystemInit(void);
void EmulatorSystemDestroy(void);
void EmulatorSystemUpdate(void);
void EmulatorSystemWaitEvent(PEvent esic_event);
BOOL EmulatorSystemPollEvent(PEvent esic_event);
void EmulatorSystemDelay(DWORD milliseconds);
void EmulatorSystemPushEvent(PEvent esic_event);
DWORD EmulatorSystemGetTicks(void);
const BYTE* EmulatorSystemGetKeyboardState();



/* Vtables definition */



#endif /* _EMULATOR_SYSTEM_ */