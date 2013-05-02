#include "debug_frmwrk.h"
#include "LPC177x_8x.h"
#include "monitor.h"


void put_char(unsigned char ch);
unsigned char get_char(void);

void debug_module_init(void) {
	debug_frmwrk_init();
	xfunc_out = put_char;
	xfunc_in  = get_char;
}


void put_char(unsigned char ch)
{
    _DBC(ch);    
}
unsigned char get_char(void)
{
   return _DG;    
}
