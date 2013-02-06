#include <esic/egui/ui_factory.h>
#include <libroxml/roxml.h>

PWidget UIFactory_loadFromXmlFile(const char* file_name) {
	node_t* document = NULL;
	PWidget new_widget = NULL;
	FIL ui_file;
	char* tmp_file_name = NULL;
	int childs_nb;

	/* First: build final path */
	tmp_file_name = (char*)SicAlloc( (strlen(UI_PATH) + strlen(file_name) + 1) * sizeof(char)); 

	/* Ensure allocation succeeded */
	assert(tmp_file_name);

	strcpy(tmp_file_name, UI_PATH);
	strcat(tmp_file_name, file_name);

	/* First: try to load the xml file */
	/* TODO: use fatfs instead */
	document = roxml_load_doc((char*)file_name);

	if(document == NULL) {
		fprintf(stderr, "Can't load %s file!\n", file_name);
		return NULL;
	}

	childs_nb = roxml_get_chld_nb(document);

}