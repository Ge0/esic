# test
CC       = gcc -m32

EAPI_O   =	eapi/src/event.o \
        	eapi/src/raster_font.o \
        	eapi/src/raster_font_table.o \
        	eapi/src/raster_icon_factory.o \
        	eapi/src/heap.o \
        	eapi/src/raster_font_factory.o \
        	eapi/src/raster_icon.o \
        	eapi/src/raster_icon_header.o

EGRAPHICS_O =	egraphics/src/ellipse.o \
		egraphics/src/triangle.o \
		egraphics/src/vertice.o

ELCD_O =	elcd/src/lcd.o \
		elcd/src/lcd_painter.o

EGUI_O =	egui/src/checkbox.o \
		egui/src/default_widget_renderer.o \
		egui/src/label.o \
		egui/src/picture.o \
		egui/src/textbox.o \
		egui/src/widget.o \
		egui/src/widget_ptr.o

ETOOLS_O =	etools/src/container.o \
		etools/src/hashnode.o \
		etools/src/hashtable.o \
		etools/src/list.o \
		etools/src/map.o \
		etools/src/vector.o \
		etools/src/zstring_buffer.o \
		etools/src/zstring.o

EXPAT_O =	expat/src/xmlparse.o \
		expat/src/xmlrole.o \
		expat/src/xmltok.o \
		expat/src/xmltok_impl.o \
		expat/src/xmltok_ns.o

FATFS_O =	fatfs/src/ff.o \
		fatfs/src/option/unicode.o

EMULATOR_O =	emulator/src/main.o \
		emulator/src/emulator_system.o \
		emulator/src/emulator_lcd.o \
		emulator/src/fatfs/diskio_emulator.o

E11_O =		e11/src/e11.o \
		e11/src/e11_ui.o \
		e11/src/xml_ui_factory.o \
		e11/src/ui/main_ui.o \
		e11/src/ui/dotpen_ui.o

all: e11emu


egraphics:
	make -C egraphics

egui:
	make -C egui

etools:
	make -C etools

elcd:
	make -C elcd

eapi:
	make -C eapi

expat:
	make -C expat

fatfs:
	make -C fatfs

emulator:
	make -C emulator

e11:
	make -C e11

e11emu: e11 emulator fatfs expat eapi etools elcd egraphics egui
	$(CC) -o e11emu $(E11_O) $(EAPI_O) $(EGRAPHICS_O) $(ELCD_O) $(EGUI_O) $(ETOOLS_O) $(EXPAT_O) $(FATFS_O) $(EMULATOR_O) -lSDL

clean:
	make -C eapi clean
	make -C e11 clean
	make -C egraphics clean
	make -C elcd clean
	make -C expat clean
	make -C fatfs clean
	make -C egui clean
	make -C etools clean
	make -C emulator clean

.PHONY: all egraphics egui etools elcd eapi expat fatfs emulator e11 e11emu clean

