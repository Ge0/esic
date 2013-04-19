#ifndef _EMULATOR_LCD_H_
#define _EMULATOR_LCD_H_

//#include <SDL/SDL.h>

void EmulatorLcdInit(DWORD width, DWORD height, BYTE bpp, DWORD flags);
void EmulatorLcdDestroy();
void EmulatorLcdUpdate();
void EmulatorLcdSetPixel(DWORD x, DWORD y, DWORD color);
void EmulatorLcdDrawLine(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);

#endif /* _LCD_EMULATOR_H_ */
