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
#endif




#endif /* _ESIC_SYSTEM_H_ */