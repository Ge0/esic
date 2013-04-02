#include <esic/elcd/lcd.h>

static SDL_Surface* s_screen = NULL;
static void* s_framebuffer   = NULL;
static BYTE s_bytes_per_pixels = 3;

/* Screen size */
static DWORD s_width = 0;
static DWORD s_height = 0;

void LcdEmulatorInit(DWORD width, DWORD height, BYTE bpp, DWORD flags) {
	DWORD internal_flags = SDL_HWSURFACE;
	int error = 0;
	
	error = SDL_Init(SDL_INIT_VIDEO);

	if(flags & LCD_DOUBLEBUF) {
		internal_flags |= SDL_DOUBLEBUF;
	}

	s_screen = SDL_SetVideoMode(width, height, bpp, internal_flags);
	_LcdInit(width, height, bpp, (void*)s_screen->pixels);

	//assert(s_screen != NULL);

	s_framebuffer = (void*)s_screen->pixels;

}

void LcdEmulatorDestroy() {
	SDL_Quit();
}

void LcdEmulatorUpdate() {
	SDL_Flip(s_screen);
}