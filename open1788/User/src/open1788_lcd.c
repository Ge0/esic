#include <esic/elcd/lcd.h>
#include "LPC177x_8x.h"
#include "EX_SDRAM.h"
#include <stdio.h>



static void _glcd_ctrl(BOOL enable);
static void _glcd_init(BYTE bpp);
static int _glcd_get_bpp_value(BYTE bpp);

void Open1788LcdInit(DWORD width, DWORD height, BYTE bpp, DWORD flags) {
	/* TODO */
	_glcd_ctrl(FALSE);
	_glcd_init(bpp);
	_glcd_ctrl(TRUE);
	
	_LcdInit(width, height, bpp, (void*)LCD_VRAM_BASE_ADDR);
}

void Open1788LcdDestroy() {
	/* TODO. */
}

void Open1788LcdUpdate() {
	/* TODO. */
}

static void _glcd_ctrl(BOOL enable) {
	volatile DWORD i;
	if (enable) {
		LPC_LCD->CTRL |= (1<<0);
		for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--) {
			LPC_LCD->CTRL |= (1<<11);
		}
	} else {
		LPC_LCD->CTRL &= ~(1<<11);
		for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
		LPC_LCD->CTRL &= ~(1<<0);
	}
}

static void _glcd_init(BYTE bpp) {
	unsigned long i;
	/*
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
	unsigned long *pDst16;
	
	const unsigned long *pBmp16 = pBmp16;
	*/
	
	/* R */
	LPC_IOCON->P0_4  = 0x27;	 // LCD_VD_0
	LPC_IOCON->P0_5   = 0x27;  // LCD_VD_1
	LPC_IOCON->P4_28   = 0x27;  // LCD_VD_2

	LPC_IOCON->P4_29  = 0x27;	 // LCD_VD_3
	LPC_IOCON->P2_6   = 0x27;  // LCD_VD_4
	LPC_IOCON->P2_7   = 0x27;  // LCD_VD_5
	LPC_IOCON->P2_8   = 0x27;  // LCD_VD_6
	LPC_IOCON->P2_9  = 0x27;  // LCD_VD_7 

	/* G */     
	LPC_IOCON->P0_6  = 0x27;  // LCD_VD_8
	LPC_IOCON->P0_7  = 0x27;  // LCD_VD_9


	LPC_IOCON->P1_20  = 0x27;  // LCD_VD_10
	LPC_IOCON->P1_21  = 0x27;  // LCD_VD_11
	LPC_IOCON->P1_22  = 0x27;  // LCD_VD_12
	LPC_IOCON->P1_23  = 0x27;  // LCD_VD_13
	LPC_IOCON->P1_24  = 0x27;  // LCD_VD_14
	LPC_IOCON->P1_25  = 0x27;  // LCD_VD_15

	/* B */
	LPC_IOCON->P0_8  = 0x27;  // LCD_VD_16
	LPC_IOCON->P0_9  = 0x27;  // LCD_VD_17
	LPC_IOCON->P2_12  = 0x27;  // LCD_VD_18

	LPC_IOCON->P2_13  = 0x27;	 // LCD_VD_19
	LPC_IOCON->P1_26  = 0x27;  // LCD_VD_20
	LPC_IOCON->P1_27  = 0x27;  // LCD_VD_21
	LPC_IOCON->P1_28  = 0x27;  // LCD_VD_22
	LPC_IOCON->P1_29  = 0x27;  // LCD_VD_23

	LPC_IOCON->P2_2   = 0x27;  // LCD_DCLK
	LPC_IOCON->P2_5   = 0x27;  // LCD_LP -- HSYNC
	LPC_IOCON->P2_3   = 0x27;  // LCD_FP -- VSYNC
	LPC_IOCON->P2_4   = 0x27;  // LCD_ENAB_M -- LCDDEN
	LPC_IOCON->P2_0   = 0x27;  // LCD_PWR

	// <<< debug <<<

	/*Back light enable*/
	LPC_GPIO2->DIR = (1<<1);
	LPC_GPIO2->SET= (5<<1);

	//Turn on LCD clock
	LPC_SC->PCONP |= 1<<0;

	// Disable cursor
	LPC_LCD->CRSR_CTRL &=~(1<<0);

	// disable GLCD controller	
	LPC_LCD->CTRL = 0;

	LPC_LCD->CTRL &= ~(0x07 <<1);
	//	#ifndef __RAM__
	// RGB565
	//	LPC_LCD->CTRL |= (6<<1);
	//	#else
	// RGB888
	LPC_LCD->CTRL |= (_glcd_get_bpp_value(bpp)<<1);
	//	#endif

	// TFT panel
	LPC_LCD->CTRL |= (1<<5);
	// single panel
	LPC_LCD->CTRL &= ~(1<<7);
	// notmal output
	LPC_LCD->CTRL &= ~(1<<8);
	// little endian byte order
	LPC_LCD->CTRL &= ~(1<<9);
	
	// GR: big endian byte order
	//LPC_LCD->CTRL |= (1<<9);
	
	// little endian pix order
	LPC_LCD->CTRL &= ~(1<<10);
	
	
	// GR: big endian pix order
	//LPC_LCD->CTRL |= (1<<10);
	
	
	// disable power
	LPC_LCD->CTRL &= ~(1<<11);
	// init pixel clock
	LPC_SC->LCD_CFG = PeripheralClock / ((unsigned long) C_GLCD_PIX_CLK);
	// bypass inrenal clk divider
	LPC_LCD->POL |=(1<<26);
	// clock source for the LCD block is HCLK
	LPC_LCD->POL &= ~(1<<5);
	// LCDFP pin is active LOW and inactive HIGH
	LPC_LCD->POL |= (1<<11);
	// LCDLP pin is active LOW and inactive HIGH
	LPC_LCD->POL |= (1<<12);
	// data is driven out into the LCD on the falling edge
	LPC_LCD->POL &= ~(1<<13);
	// active high
	LPC_LCD->POL &= ~(1<<14);
	LPC_LCD->POL &= ~(0x3FF <<16);
	LPC_LCD->POL |= (C_GLCD_H_SIZE-1)<<16;

	// init Horizontal Timing
	LPC_LCD->TIMH = 0; //reset TIMH before set value
	LPC_LCD->TIMH |= (C_GLCD_H_BACK_PORCH - 1)<<24;
	LPC_LCD->TIMH |= (C_GLCD_H_FRONT_PORCH - 1)<<16;
	LPC_LCD->TIMH |= (C_GLCD_H_PULSE - 1)<<8;
	LPC_LCD->TIMH |= ((C_GLCD_H_SIZE/16) - 1)<<2;

	// init Vertical Timing
	LPC_LCD->TIMV = 0;  //reset TIMV value before setting
	LPC_LCD->TIMV |= (C_GLCD_V_BACK_PORCH)<<24;
	LPC_LCD->TIMV |= (C_GLCD_V_FRONT_PORCH)<<16;
	LPC_LCD->TIMV |= (C_GLCD_V_PULSE - 1)<<10;
	LPC_LCD->TIMV |= C_GLCD_V_SIZE - 1;
	// Frame Base Address doubleword aligned
	LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	LPC_LCD->LPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	// init colour pallet

	for(i = C_GLCD_ENA_DIS_DLY; i; i--);
}
		
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

static int _glcd_get_bpp_value(BYTE bpp) {
	int value = 0;
	switch(bpp) {
	case 1:
		value = 0;
		break;
	case 2:
		value = 1;
		break;
	case 4:
		value = 2;
		break;
	case 8:
		value = 3;
		break;
	case 16:
		value = 6;
		break;
	case 24:
		value = 5;
		break;
	}
	return value;
		
}