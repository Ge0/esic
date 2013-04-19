#ifndef _ESIC_SYSTEM_H_
#define _ESIC_SYSTEM_H_

#include <esic/esic.h>
#include <esic/eapi/event.h>

#if defined(EMULATOR)
#include <esic/eapi/emulator_system.h>
#define EsicInit				EmulatorSystemInit
#define EsicDestroy				EmulatorSystemDestroy
#define EsicWaitEvent			EmulatorSystemWaitEvent
#define EsicPollEvent			EmulatorSystemPollEvent
#define EsicDelay				EmulatorSystemDelay
#define EsicPushEvent			EmulatorSystemPushEvent
#define EsicGetTicks			EmulatorSystemGetTicks
#define EsicGetKeyboardState	EmulatorSystemGetKeyboardState
#define EsicUpdate				EmulatorSystemUpdate
#elif defined(OPEN1788)
#include <esic/eapi/open1788_system.h>
#define EsicInit				Open1788SystemInit
#define EsicDestroy				Open1788SystemDestroy
#define EsicWaitEvent			Open1788WaitEvent
#define EsicPollEvent			Open1788PollEvent
#define EsicDelay				Open1788Delay
#define EsicPushEvent			Open1788PushEvent
#define EsicGetTicks			Open1788GetTicks
#define EsicGetKeyboardState	Open1788GetKeyboardState
#define EsicUpdate				Open1788Update
#endif




#endif /* _ESIC_SYSTEM_H_ */