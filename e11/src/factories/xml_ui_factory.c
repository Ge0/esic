#include <factories/xml_ui_factory.h>

#define XML_STATIC
#include <expat/expat.h>

#include <esic/egui/widget_ptr.h>
#include <esic/etools/list.h>
#include <fatfs/ff.h>

/* Available widgets */
#include <esic/egui/checkbox.h>
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>
#include <esic/egui/graphicsview/canvas.h>



#include <string.h>

static void _start_element(void *user_data, const char *name, const char **atts);
static void _end_element(void *user_data, const char *name);

void _hydrate_widget(PWidget widget, const char** atts);
void _hydrate_checkbox(PCheckBox checkbox, const char** atts);
void _hydrate_label(PLabel label, const char** atts);
void _hydrate_textbox(PTextBox textbox, const char** atts);
void _hydrate_canvas(PCanvas canvas, const char** atts);

void XmlUiFactory_hydrateUI(const char* ui_name, PWidget widget, PAbstractPainter painter) {
	char* path = NULL;
	FIL ui_file;
	XML_Parser parser = NULL;
	int done;
	UINT br;
	char buf[BUFSIZ];

	/* TODO. */

	/* Build path */
	path = (char*)SicAlloc((strlen(PATH_USER_INTERFACES) + strlen(ui_name) + 5) * sizeof(char)); /* +5 = ".xml\0" */
	
	SicAssert(path != NULL);
	
	strcpy(path, PATH_USER_INTERFACES);
	strcat(path, ui_name);
	strcat(path, ".xml");

	//if((fp = fopen(path, "r")) == NULL) {

	if(f_open(&ui_file, path, FA_READ) != FR_OK) {
		/* Error... */
		SicPrintfDebug("Cannot open %s! Abort...\r\n", path);
		SicFree(path);
		return;
	}


	parser = XML_ParserCreate(NULL);

	/* Set our widget as the user data so the callback would be able to hydrate it */
	XML_SetUserData(parser, (void*)&widget);

	/* Test */
	widget->painter = painter;

	XML_SetElementHandler(parser, _start_element, _end_element);
	do {
		//int len = fread(buf, sizeof(char), sizeof(buf), fp);
		f_read(&ui_file, buf, sizeof(buf) * sizeof(char), &br);
		done = br < sizeof(buf);
		if(XML_Parse(parser, buf, br, done) == XML_STATUS_ERROR) {
			SicPrintfDebug("XML_Parse error.\r\n");
			return;
		}
	} while(!done);
	XML_ParserFree(parser);
	SicFree(path);
	f_close(&ui_file);
}

static void _start_element(void *user_data, const char *name, const char **atts)
{
	//SicPrintfDebug("_start_element %s\r\n", name);
	PPWidget p_widget = (PPWidget)user_data;

	if(strcmp(name, "window") == 0) {
		/* Continue the parsing */
		return;
	} else {
		PWidget new_widget = NULL;

		/* Given the type of the widget, instantiate the appropriated structure */
		if(strcmp(name, "label") == 0) {
			PLabel new_label;
			NEW(new_label, Label);
			_hydrate_label(new_label, atts);
			new_widget = (PWidget)new_label;
		} else if(strcmp(name, "textbox") == 0) {
			PTextBox new_textbox;
			NEW(new_textbox, TextBox);
			_hydrate_textbox(new_textbox, atts);
			new_widget = (PWidget)new_textbox;
		} else if(strcmp(name, "checkbox") == 0) {
			PCheckBox new_checkbox;
			NEW(new_checkbox, CheckBox);
			_hydrate_checkbox(new_checkbox, atts);
			new_widget = WIDGET(new_checkbox);
		} else if(strcmp(name, "canvas") == 0) {
			PCanvas new_canvas;
			NEW(new_canvas, Canvas);
			//_hydrate_canvas(new_canvas, atts);
			new_widget = WIDGET(new_canvas);
		}



		/* If the widget is constructed, hydrate it */
		if(new_widget != NULL) {
			_hydrate_widget(new_widget, atts);

			/* Associate the relationship */
			new_widget->parent = *p_widget;
			new_widget->painter = (*p_widget)->painter;

			/* Append to the list of childs of the current widget */
			Widget_addChild(*p_widget, new_widget, TRUE);

			/* Updates the current parent widget as the one that has just been pushed back */
			*p_widget = ((PWidgetPtr)List_tail(&(*p_widget)->childs))->widget;
		}
	}
}

static void _end_element(void *user_data, const char *name) {
	/* Updates the current parent */
	PPWidget p_widget = (PPWidget)user_data;
	if((*p_widget)->parent != NULL) {
		*p_widget = (*p_widget)->parent;
	}
}

void _hydrate_widget(PWidget widget, const char** atts) {
	DWORD i;

	for (i = 0; atts[i] != NULL; i += 2) {
		if(strcmp(atts[i], "id") == 0) {
			widget->id = atoi(atts[i+1]);
		} else if(strcmp(atts[i], "x") == 0) {
			widget->x = atoi(atts[i+1]);
		} else if(strcmp(atts[i], "y") == 0) {
			widget->y = atoi(atts[i+1]);
		} else if(strcmp(atts[i], "width") == 0) {
			widget->width = atoi(atts[i+1]);
		} else if(strcmp(atts[i], "height") == 0) {
			widget->height = atoi(atts[i+1]);
		} else if(strcmp(atts[i], "color") == 0) {
			widget->color = atoi(atts[i+1]);
		}
	}
}

void _hydrate_checkbox(PCheckBox checkbox, const char** atts) {
	DWORD i;

	for(i = 0; atts[i] != NULL; i += 2) {
		//if(strcmp(
	}
}

void _hydrate_label(PLabel label, const char** atts) {
	DWORD i;

	for(i = 0; atts[i] != NULL; i += 2) {
		if(strcmp(atts[i], "caption") == 0) {
			Label_setCaption(label, atts[i+1]);
		}
	}

}

void _hydrate_textbox(PTextBox textbox, const char** atts) {
	DWORD i;

	textbox->background_color         = TEXTBOX_DEFAULT_BACKGROUND_COLOR;
	textbox->border_color             = TEXTBOX_DEFAULT_BORDER_COLOR;
	textbox->focused_background_color = TEXTBOX_DEFAULT_FOCUSED_BACKGROUND_COLOR;
	textbox->focused_border_color     = TEXTBOX_DEFAULT_FOCUSED_BORDER_COLOR;

	for(i = 0; atts[i]; i += 2) {
		if(strcmp(atts[i], "text") == 0) {
			TextBox_setText(textbox, atts[i+1]);
		} else if(strcmp(atts[i], "background_color") == 0) {
			DWORD color  = atoi(atts[i+1]);
			WORD  wcolor = RGB_16B(RED(color), GREEN(color), BLUE(color));
			textbox->background_color = wcolor;
		} else if(strcmp(atts[i], "border_color") == 0) {
			DWORD color  = atoi(atts[i+1]);
			WORD  wcolor = RGB_16B(RED(color), GREEN(color), BLUE(color));
			textbox->border_color = wcolor;
		} else if(strcmp(atts[i], "focused_background_color") == 0) {
			DWORD color  = atoi(atts[i+1]);
			WORD  wcolor = RGB_16B(RED(color), GREEN(color), BLUE(color));
			textbox->focused_background_color = wcolor;
		} else if(strcmp(atts[i], "focused_border_color") == 0) {
			DWORD color  = atoi(atts[i+1]);
			WORD  wcolor = RGB_16B(RED(color), GREEN(color), BLUE(color));
			textbox->focused_border_color = wcolor;
		}
	}

}
