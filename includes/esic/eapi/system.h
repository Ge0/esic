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
#elif defined(__LPC177X_8X__)
#include <esic/eapi/lpc1788_system.h>
#define EsicInit				Lpc1788SystemInit
#define EsicDestroy				Lpc1788SystemDestroy
#define EsicWaitEvent			Lpc1788WaitEvent
#define EsicPollEvent			Lpc1788PollEvent
#define EsicDelay				Lpc1788Delay
#define EsicPushEvent			Lpc1788PushEvent
#define EsicGetTicks			Lpc1788GetTicks
#define EsicGetKeyboardState	Lpc1788GetKeyboardState
#define EsicUpdate				Lpc1788Update
#endif




#endif /* _ESIC_SYSTEM_H_ */