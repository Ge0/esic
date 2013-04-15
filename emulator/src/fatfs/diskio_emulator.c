/*-----------------------------------------------------------------------*/
/* Low level disk control module for Win32              (C)ChaN, 2012    */
/*-----------------------------------------------------------------------*/



#include <stdio.h>
#include "diskio_emulator.h"
#include <fatfs/ff.h>


#define MAX_DRIVES	10	/* Max number of physical drives to be used */

#define SZ_RAMDISK	64	/* Size of RAM disk [MB] */
#define SS_RAMDISK	512	/* Sector size of RAM disk [byte] */


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

#define	BUFSIZE 65536UL	/* Size of data transfer buffer */

typedef struct {
	DSTATUS	status;
	WORD sz_sector;
	DWORD n_sectors;
	FILE* fp;
} STAT;

static HANDLE hMutex, hTmrThread;
static int Drives;

static volatile STAT Stat[MAX_DRIVES];


static DWORD TmrThreadID;

static BYTE *Buffer, *RamDisk;	/* Poiter to the data transfer buffer and ramdisk */


int get_status (
	BYTE drv
)
{
	return 1;
}




/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Windows disk accesss layer                                 */
/*-----------------------------------------------------------------------*/

int diskio_assign_drive (const char* filename, WORD size_sector, DWORD n_sectors)
{
	Stat[0].fp = fopen(filename, "rb+");
	Stat[0].sz_sector = size_sector;
	Stat[0].n_sectors = n_sectors;

	return Stat[0].fp != NULL;
}


void  diskio_destroy_drive(void)
{
	fclose(Stat[0].fp);
}


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{

	if(drv != 0) {
		return STA_NOINIT;
	}

	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	DWORD nc, rnc;
	LARGE_INTEGER ofs;
	DSTATUS res;

	nc = (DWORD)count * Stat[drv].sz_sector;
	ofs.QuadPart = (LONGLONG)sector * Stat[drv].sz_sector;

	fseek(Stat[0].fp, (DWORD)sector * Stat[0].sz_sector, SEEK_SET);
	fread(buff, sizeof(BYTE), nc, Stat[0].fp);

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	DWORD nc, rnc;
	LARGE_INTEGER ofs;
	DSTATUS res;

	nc = (DWORD)count * Stat[drv].sz_sector;
	ofs.QuadPart = (LONGLONG)sector * Stat[drv].sz_sector;

	fseek(Stat[0].fp, (DWORD)sector * Stat[0].sz_sector, SEEK_SET);
	fwrite(buff, sizeof(BYTE), nc, Stat[0].fp);

	return RES_OK;
}
#endif /* _USE_WRITE */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	DRESULT res = RES_PARERR;
	switch (ctrl) {
	case CTRL_SYNC:
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		*(DWORD*)buff = Stat[0].n_sectors;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:
		*(WORD*)buff = Stat[0].sz_sector;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
		*(DWORD*)buff = 128;
		res = RES_OK;
		break;

	}

	return res;
}
#endif /* _USE_IOCTL */

