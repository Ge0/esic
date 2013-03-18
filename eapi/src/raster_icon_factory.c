/**
 * \file raster_icon_factory.c
 */
#include <assert.h>
#include <esic/eapi/heap.h>
#include <esic/eapi/raster_icon_factory.h>
#include <esic/etools/hashtable.h>
#include <esic/etools/szstring.h>
#include <fatfs/ff.h>

static Hashtable s_icons;

PRasterIcon _build_raster_icon(const char* name);

void RasterIconFactory_init() {
	Hashtable_constructor(&s_icons, 5, sizeof(SzString), sizeof(RasterIcon));
}

void RasterIconFactory_destroy() {
	Hashtable_destructor(&s_icons.map.object);
}

PRasterIcon RasterIconFactory_getRasterIcon(const char* icon_name) {
	SzString key;
	PRasterIcon return_icon = NULL;
	/* First: check whether the font has been loaded into memory or not */
	SzString_constructor(&key, icon_name);

	if(!Hashtable_containsKey(&s_icons.map, &key.object)) {
		/* If not, load the font & add it to the map */
		return_icon = _build_raster_icon(icon_name);

		if(return_icon != NULL) {
			/* Put in the hashtable & point into it */
			Hashtable_put(&s_icons.map, &key.object, &return_icon->object);
			DELETE(return_icon);
			
		}
	}

	/* Finally: get the font into the hashtable */
	return_icon = (PRasterIcon)Hashtable_get(&s_icons.map, &key.object);

	SzString_destructor(&key.object);

	return return_icon;
}


PRasterIcon _build_raster_icon(const char* name) {
	PRasterIcon return_icon = NULL;
	char* tmp_path = NULL;
	FIL icon_file;
	UINT br;
	DWORD size_data;

	tmp_path = (char*)SicAlloc((strlen(PATH_RASTER_ICONS) + strlen(name) + 1) * sizeof(char));
	assert(tmp_path != NULL);

	strcpy(tmp_path, PATH_RASTER_ICONS);
	strcat(tmp_path, name);

	if(f_open(&icon_file, (const TCHAR*)tmp_path, FA_READ) != FR_OK) {
		/* Error... */
		fprintf(stderr, "Can't open %s!\n", name);
		SicFree(tmp_path);
		return NULL;
	}

	NEW(return_icon, RasterIcon);

	

	/* Read the header */
	f_read(&icon_file, &return_icon->header, sizeof(RasterIconHeader), &br);

	if(return_icon->header.signature != RASTER_ICON_MAGIC_WORD) {
		/* Bad signature, free the memory & exit */
		SicFree(return_icon);
		SicFree(tmp_path);
		return NULL;
	}

	size_data = return_icon->header.width * return_icon->header.height * sizeof(WORD);
	return_icon->data = (BYTE*)SicAlloc(size_data);

	/* Ensure allocation succeeded */
	assert(return_icon->data != NULL);

	/* Read the content */
	f_read(
		&icon_file,
		return_icon->data,
		size_data,
		&br
	);


	SicFree(tmp_path);

	/* & return the icon */
	return return_icon;
}