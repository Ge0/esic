#include "SDL/SDL.h"
#include "emulator_system.h"
#include <e11.h>

//#if defined (Win32)
#include <crtdbg.h>
//#endif

int main(int argc, char** argv) {

	EmulatorSystem sys;

	EmulatorSystem_constructor(&sys);

	e11(&sys.abstract_system);

	EmulatorSystem_destructor(&sys.abstract_system.object);

//#if defined (Win32)
	_CrtDumpMemoryLeaks();
//#endif

	return EXIT_SUCCESS;
}