#ifndef _PREVIEW_UI_H_
#define _PREVIEW_UI_H_

#include <e11_ui.h>
#include <esic/egui/graphicsview/graphics_scene.h>
#include <libsic/marking/text_marking_line.h>

#define PREVIEW_UI_ID_CANVAS 5

typedef struct _PreviewUI* PPreviewUI;
typedef struct _PreviewUI {
	E11UI e11ui;

	GraphicsScene marking_file_scene;

	// Test
	TextMarkingLine test_marking_line;

} PreviewUI;

PPreviewUI PreviewUI_constructor(PPreviewUI self);

/* Virtual functions */
/* Object */
void PreviewUI_destructor(PObject self);

/* Widget */
void PreviewUI_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y);
DWORD PreviewUI_defaultProc(PWidget self, const PEvent event);

/* E11 UI */
#define E11_UI_FUNCTION(keycode) void PreviewUI_on##keycode(PE11UI);
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION

#define E11_PREVIEWUI(x) ((PPreviewUI)x)

#endif /* _PREVIEW_UI_H */