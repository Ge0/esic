#include "fat_module.h"
#include "monitor.h"
#include "diskio.h"
#include "ff.h"

static FATFS fs;

void fat_init(void) {
	FRESULT fr;
	DSTATUS ds;
	

	xprintf("[*] disk_initialize(0): ");
	if((ds = disk_initialize(0)) != RES_OK ) {
		xprintf("Error.\r\n");
		return;
	}
	xprintf("OK!\r\n");
	
	xprintf("[*] Mounting the SD card... ");
	if((fr = f_mount(0, &fs)) != FR_OK) {
		xprintf("Error.\r\n");
		return;
	}
	
	xprintf("OK!\r\n");
}
