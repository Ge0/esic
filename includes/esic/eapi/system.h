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
#define EsicWaitEvent			Open1788SystemWaitEvent
#define EsicPollEvent			Open1788SystemPollEvent
#define EsicDelay				Open1788SystemDelay
#define EsicPushEvent			Open1788SystemPushEvent
#define EsicGetTicks			Open1788SystemGetTicks
#define EsicGetKeyboardState	Open1788SystemGetKeyboardState
#define EsicUpdate				Open1788SystemUpdate
#endif




#endif /* _ESIC_SYSTEM_H_ */