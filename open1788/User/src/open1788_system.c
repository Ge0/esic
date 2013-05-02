#include <esic/eapi/system.h>
#include <esic/elcd/lcd.h>

#include "fat_module.h"
#include "debug_module.h"
#include "EX_SDRAM.h"
#include "timer.h"
#include "system_queue.h"

/*
static void FatInit(FATFS* fs);
static void TestEnumDirs(FATFS* fs);


void put_char(unsigned char ch);
unsigned char get_char(void);
*/

extern volatile uint32_t timer0_counter;

void  Open1788SystemInit(void) {
	/* TODO. */
	
	debug_module_init();
	
	fat_init();

	SDRAM_Init();
	
	enable_timer(0);
	init_timer(0, (SystemCoreClock / 1000));
	
	LcdInit(480, 272, 16, LCD_DOUBLEBUF);
	LcdFill(RGB_16B(240,240,240));
	
	
}

void  Open1788SystemDestroy(void) {
	/* TODO. */
	
	/* Unmount the SD card? */
}


void  Open1788SystemWaitEvent(PEvent esic_event) {
	while(SizeSystemQueue() == 0);
	
	/* TODO. */
}


BOOL  Open1788SystemPollEvent(PEvent esic_event) {
	/* TODO. */
	BOOL ret = FALSE;
	__disable_interrupt();
	if(SizeSystemQueue()) {
		ret = (BOOL)(DequeueSystemQueue(esic_event) == 1);
	}
	__enable_interrupt();
	return ret;
}

void  Open1788SystemDelay(DWORD milliseconds) {
	/* TODO. */
}

void  Open1788SystemPushEvent(const PEvent esic_event) {
	/* TODO. */
	__disable_interrupt();
	EnqueueSystemQueue(esic_event);
	__enable_interrupt();
}

DWORD Open1788SystemGetTicks(void) {
	/* TODO. */
	return timer0_counter;
}

const BYTE* Open1788SystemGetKeyboardState() {
	/* TODO. */
	return NULL;
}

/*
void FatInit(FATFS* fs) {
	FRESULT fr;
	DSTATUS ds;
	
	xprintf("[*] disk_initialize(0): ");
	if((ds = disk_initialize(0)) != RES_OK ) {
		xprintf("Error.\r\n");
		return;
	}
	xprintf("OK!\r\n");
	
	_DBG("[*] Mounting the SD card... ");
	if((fr = f_mount(0, fs)) != FR_OK) {
		xprintf("Error.\r\n");
		return;
	}
	
	_DBG("OK!\r\n");
	
	
}

void TestEnumDirs(FATFS* fs) {
	
	FILINFO filinfo;
	DIR root_dir;

	xprintf("Opening 0:/... ");
	if(f_opendir(&root_dir, "0:/") != FR_OK) {
		_DBG("Error.\r\n");
		return;
	}
	
	xprintf("OK!\r\n");
	
	while(f_readdir(&root_dir, &filinfo) == FR_OK) {
		if(filinfo.fname[0] == '\0') {
			break;
		}
		xprintf(filinfo.fname);
		xprintf("\r\n");
	}
		
	
		
	
}
*/
