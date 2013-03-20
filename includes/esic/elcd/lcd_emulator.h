#ifndef _LCD_EMULATOR_H_
#define _LCD_EMULATOR_H_

#include <SDL/SDL.h>

void LcdEmulatorInit(DWORD width, DWORD height, BYTE bpp, DWORD flags);
void LcdEmulatorDestroy();
void LcdEmulatorUpdate();
void LcdEmulatorSetPixel(DWORD x, DWORD y, DWORD color);
void LcdEmulatorDrawLine(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);

#endif /* _LCD_EMULATOR_H_ */