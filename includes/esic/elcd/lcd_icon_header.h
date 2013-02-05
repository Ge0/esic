#ifndef _LCD_ICON_HEADER_H_
#define _LCD_ICON_HEADER_H_

#include <esic/e11_types.h>

typedef struct _LcdIconHeader {
	DWORD signature;
	WORD  version;
	WORD  width;
	WORD  height;
} LcdIconHeader;

// foo

#endif /* _LCD_ICON_HEADER_H_ */