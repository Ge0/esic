/**
 * \file main.c
 */
#include <esic/eapi/system.h>
#include <e11.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif


extern int assign_drives(void);
//extern VOID StopTmrThread();

int main(int argc, char** argv) {

	EsicInit();

	e11();

	EsicDestroy();

#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif

	return EXIT_SUCCESS;
}
