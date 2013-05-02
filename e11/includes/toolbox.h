#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include <esic/egui/widget.h>
#include <esic/eresources/raster_icon.h>

#define NUMBER_OF_ICONS 12

typedef struct _ToolBox {
	PWidget widget; /* ? */
	RasterIcon icons[NUMBER_OF_ICONS];
} ToolBox, *PToolBox;

PToolBox ToolBox_constructor(PToolBox self);
PRasterIcon ToolBox_getIcon(PToolBox self, DWORD index);

/* Virtual functions */
/* Object */

/* Widget */

#endif /* _TOOLBOX_H_ */