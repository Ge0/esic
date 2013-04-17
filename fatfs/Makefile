SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR

# diskio.c  emulator_system.c  ff.c

OBJ =	$(PATH_SRC)/ff.o \
	$(PATH_SRC)/option/unicode.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
