#ifndef _LCD_FONT_FACTORY_H_
#define _LCD_FONT_FACTORY_H_


#include "lcd_font.h"

//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif

//PLcdFont LcdFactory_getLcdFont(const char* font_name);
PLcdFont build_font(const char* name); // Temp

#endif /* _LCD_FACTORY_H_ */