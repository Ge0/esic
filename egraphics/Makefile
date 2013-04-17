SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR
# ellipse.c  rectangle.c  triangle.c  vertice.c
OBJ =	$(PATH_SRC)/ellipse.o \
	$(PATH_SRC)/triangle.o \
	$(PATH_SRC)/vertice.o \

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
