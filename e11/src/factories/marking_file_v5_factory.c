#include <fatfs/ff.h>
#include <libsic/marking/marking_file.h>
#include <factories/marking_file_v5_factory.h>


PMarkingFile _load_file_v5(const char* filename) {
	FIL fp;
	char magic[MAGIC_WORD_V5_SIZE];
	UINT br; /* Number of bytes read */
	BYTE number_of_variables, number_of_lines, number_of_options;

	if(f_open(&fp, filename, FA_READ) != FR_OK) {
		SicPrintfDebug("Cannot open %s. Skipping...\r\n");
		return NULL;
	}

	f_read(&fp, magic, MAGIC_WORD_V5_SIZE, &br);

	if(memcmp(magic, MAGIC_WORD_V5, MAGIC_WORD_V5_SIZE) != 0) {
		SicPrintfDebug("%s is not a valid V5 marking file. Skipping...\r\n");
		f_close(&fp);
	}


	f_lseek(&fp, 1); /* bypass a useless byte */

	/* Reading numbers */
	f_read(&fp, &number_of_variables, 1, &br);
	f_read(&fp, &number_of_lines, 1, &br);
	f_read(&fp, &number_of_options, 1, &br);

	f_lseek(&fp, 1); /* bypass a useless byte */



}