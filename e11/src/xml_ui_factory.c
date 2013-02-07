#include "xml_ui_factory.h"

#define XML_STATIC
#include <expat/expat.h>

#include <esic/etools/list.h>

/* Available widgets */
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>

static const vtable_AbstractUIFactory s_vtable_abstract_ui_factory = {
	XmlUiFactory_getUI
};

static void _start_element(void *user_data, const char *name, const char **atts);
static void _end_element(void *user_data, const char *name);

void _hydrate_widget(PWidget widget, const char** atts);

PWidget XmlUiFactory_getUI(const char* ui_name) {
	PWidget built_widget = NULL;
	char* path = NULL;
	FILE* fp = NULL;
	XML_Parser parser = NULL;
	int done;
	char buf[BUFSIZ];

	/* TODO. */

	/* Build path */
	path = (char*)SicAlloc((strlen(ui_name) + 5) * sizeof(char)); /* +5 = ".xml\0" */
	strcpy(path, ui_name);
	strcat(path, ".xml");

	if((fp = fopen(path, "r")) == NULL) {
		/* Error... */
		SicFree(path);
		return NULL;
	}

	parser = XML_ParserCreate(NULL);

	/* Create the widget of the user interface */
	built_widget = NEW(built_widget, Widget);

	/* Set our widget as the user data so the callback would be able to hydrate it */
	XML_SetUserData(parser, (void*)&built_widget);

	XML_SetElementHandler(parser, _start_element, _end_element);

	do {
		int len = fread(buf, sizeof(char), sizeof(buf), fp);
		done = len < sizeof(buf);
		if(XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
			/* Error... */
			return NULL;
		}
	} while(!done);

	SicFree(path);
	fclose(fp);
	return built_widget;
}


static void _start_element(void *user_data, const char *name, const char **atts)
{
	PPWidget p_widget = (PPWidget)user_data;

	if(strcmp(name, "window") == 0) {
		/* Continue the parsing */
		return;
	} else {
		PWidget new_widget = NULL;

		/* Given the type of the widget, instantiate the appropriated structure */
		if(strcmp(name, "label") == 0) {
			PLabel new_label = (PLabel)new_widget;
			new_label = NEW(new_label, Label);
		} else if(strcmp(name, "textbox") == 0) {
			PTextBox new_textbox = (PTextBox)new_widget;
			new_textbox = NEW(new_textbox, TextBox);
		}

		/* If the widget is constructed, hydrate it */
		if(new_widget != NULL) {
			_hydrate_widget(new_widget, atts);

			/* Associate the relationship */
			new_widget->parent = *p_widget;

			/* Append to the list of childs of the current widgets */
			List_pushBack(&(*p_widget)->childs.container, (PObject)new_widget);

			/* Delete the widget since it's been added */
			DELETE(new_widget);

			/* Updates the current parent widget as the one that has just been pushed back */
			*p_widget = (PWidget)List_tail(&(*p_widget)->childs);
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

	for (i = 0; atts[i]; i += 2) {
		if(strcmp(atts[i], "x") == 0) {
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