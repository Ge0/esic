#include "SDL/SDL.h"
#include "emulator_system.h"
#include <e11.h>

//#if defined (Win32)
#include <crtdbg.h>
//#endif

extern int assign_drives(void);
extern VOID StopTmrThread();

int main(int argc, char** argv) {

	EmulatorSystem sys;

	/* Specific to Windows layer: drives assignment */
	assign_drives();

	EmulatorSystem_constructor(&sys);

	e11(&sys.abstract_system);

	EmulatorSystem_destructor(&sys.abstract_system.object);

	StopTmrThread();

//#if defined (Win32)
	_CrtDumpMemoryLeaks();
//#endif

	return EXIT_SUCCESS;
}