SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR

OBJ =	$(PATH_SRC)/checkbox.o \
	$(PATH_SRC)/default_widget_renderer.o \
	$(PATH_SRC)/label.o \
	$(PATH_SRC)/picture.o \
	$(PATH_SRC)/textbox.o \
	$(PATH_SRC)/widget.o \
	$(PATH_SRC)/widget_ptr.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
