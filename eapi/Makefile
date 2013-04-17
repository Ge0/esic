SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR

# event.c  raster_font.c          raster_font_table.c  raster_icon_factory.c
# heap.c   raster_font_factory.c  raster_icon.c        raster_icon_header.c

OBJ =	$(PATH_SRC)/event.o \
	$(PATH_SRC)/raster_font.o \
	$(PATH_SRC)/raster_font_table.o \
	$(PATH_SRC)/raster_icon_factory.o \
	$(PATH_SRC)/heap.o \
	$(PATH_SRC)/raster_font_factory.o \
	$(PATH_SRC)/raster_icon.o \
	$(PATH_SRC)/raster_icon_header.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
