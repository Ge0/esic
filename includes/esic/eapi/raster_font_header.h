/**
 * \file raster_font_header.h
 */
#ifndef _RASTER_FONT_HEADER_H_
#define _RASTER_FONT_HEADER_H_

#include <stdio.h>
#include <esic/esic.h>

typedef struct _RasterFontHeader {
	DWORD			signature;				/* Signature of a raster font file (must be: 0x0151c51c) */
	WORD			version;				/* To support extensions? */
	WORD			number_of_tables;		/* Number of tables */
} RasterFontHeader, *PRasterFontHeader;


#endif /* _RASTER_FONT_HEADER_H_ */