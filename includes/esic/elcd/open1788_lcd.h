#ifndef _OPEN1788_LCD_H_
#define _OPEN1788_LCD_H_

#define LCD_VRAM_BASE_ADDR 		((unsigned long)SDRAM_BASE + 0x00010000)
#define LCD_CURSOR_BASE_ADDR 	((unsigned long)0x20088800)

#define C_GLCD_REFRESH_FREQ     (50)	// Hz
#define C_GLCD_H_SIZE           480
#define C_GLCD_H_PULSE          2	//
#define C_GLCD_H_FRONT_PORCH    5	//
#define C_GLCD_H_BACK_PORCH     40 	//
#define C_GLCD_V_SIZE           272
#define C_GLCD_V_PULSE          2
#define C_GLCD_V_FRONT_PORCH    8
#define C_GLCD_V_BACK_PORCH     8

#define C_GLCD_PWR_ENA_DIS_DLY  10000
#define C_GLCD_ENA_DIS_DLY      10000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

#define TEXT_DEF_TAB_SIZE 5

/*
LPC178x_7x User manual.pdf page 294 of 1035
3:1 LcdBpp LCD bits per pixel:
Selects the number of bits per LCD pixel:
000 = 1 bpp.
001 = 2 bpp.
010 = 4 bpp.
011 = 8 bpp.
100 = 16 bpp.
101 = 24 bpp (TFT panel only).
110 = 16 bpp, 5:6:5 mode.
111 = 12 bpp, 4:4:4 mode.
*/

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (C_GLCD_CLK_PER_LINE * C_GLCD_LINES_PER_FRAME * C_GLCD_REFRESH_FREQ)





void Open1788LcdInit(DWORD width, DWORD height, BYTE bpp, DWORD flags);
void Open1788LcdDestroy();
void Open1788LcdUpdate();

#endif /* _OPEN1788_LCD_H_ */
