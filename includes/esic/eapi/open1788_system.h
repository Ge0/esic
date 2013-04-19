/**
 * \file open1788_system.h
 */
#ifndef _OPEN1788_SYSTEM_H_
#define _OPEN1788_SYSTEM_H_

#include <esic/eapi/event.h>


#define VOLUME_NUMBER	0
#define DEFAULT_BACKGROUND_COLOR RGB_16B(240,240,240)

#define TOTAL_SIZE 128 * (1024*1024) // 128Mo
#define SIZE_SECTOR	512

void Open1788SystemInit(void);
void Open1788SystemDestroy(void);
void Open1788SystemUpdate(void);
void Open1788SystemWaitEvent(PEvent esic_event);
BOOL Open1788SystemPollEvent(PEvent esic_event);
void Open1788SystemDelay(DWORD milliseconds);
void Open1788ystemPushEvent(PEvent esic_event);
DWORD Open1788SystemGetTicks(void);
const BYTE* Open1788SystemGetKeyboardState();

#endif /* _OPEN1788_SYSTEM_ */
