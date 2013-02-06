#ifndef _LCD_FONT_FACTORY_H_
#define _LCD_FONT_FACTORY_H_
#include "lcd_font.h"

#define PATH_LCD_FONTS "system/lcd_fonts/"

//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif

//PLcdFont LcdFactory_getLcdFont(const char* font_name);

void LcdFontFactory_init();
void LcdFontFactory_destroy();
PLcdFont LcdFontFactory_getLcdFont(const char* font_name);
 // Temp

#endif /* _LCD_FACTORY_H_ */