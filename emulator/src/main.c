/**
 * \file main.c
 */
#include <esic/eapi/system.h>
#include <e11.h>
#include <crtdbg.h>



extern int assign_drives(void);
extern VOID StopTmrThread();

int main(int argc, char** argv) {

#if defined (WIN32)
	assign_drives();
#endif

	EsicInit();

	e11();

	EsicDestroy();

	_CrtDumpMemoryLeaks();

	return EXIT_SUCCESS;
}