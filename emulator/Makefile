SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -Iincludes -DEMULATOR

OBJ =	$(PATH_SRC)/emulator_system.o \
	$(PATH_SRC)/main.o \
	$(PATH_SRC)/fatfs/diskio_emulator.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
