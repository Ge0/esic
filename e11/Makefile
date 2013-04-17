SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -Iincludes -DEMULATOR

# e11.c  e11_ui.c  testFreeType.c  toolbox.c  ui  xml_ui_factory.c

OBJ =	$(PATH_SRC)/e11.o \
	$(PATH_SRC)/e11_ui.o \
	$(PATH_SRC)/toolbox.o \
	$(PATH_SRC)/xml_ui_factory.o \
	$(PATH_SRC)/ui/main_ui.o \
	$(PATH_SRC)/ui/dotpen_ui.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o $(PATH_SRC)/ui/*.o
