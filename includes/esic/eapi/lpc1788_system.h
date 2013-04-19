/**
 * \file emulator_system.h
 */
#ifndef _LPC1788_SYSTEM_H_
#define _LPC1788_SYSTEM_H_

#include <esic/eapi/event.h>


#define VOLUME_NUMBER	0
#define DEFAULT_BACKGROUND_COLOR RGB_16B(240,240,240)

#define TOTAL_SIZE 128 * (1024*1024) // 128Mo
#define SIZE_SECTOR	512

void LPC1788SystemInit(void);
void LPC1788SystemDestroy(void);
void LPC1788SystemUpdate(void);
void LPC1788SystemWaitEvent(PEvent esic_event);
BOOL LPC1788SystemPollEvent(PEvent esic_event);
void LPC1788SystemDelay(DWORD milliseconds);
void LPC1788ystemPushEvent(PEvent esic_event);
DWORD LPC1788SystemGetTicks(void);
const BYTE* EmulatorSystemGetKeyboardState();



/* Vtables definition */



#endif /* _EMULATOR_SYSTEM_ */
