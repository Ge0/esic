SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR

OBJ =	$(PATH_SRC)/container.o \
	$(PATH_SRC)/hashnode.o \
	$(PATH_SRC)/hashtable.o \
	$(PATH_SRC)/list.o \
	$(PATH_SRC)/map.o \
	$(PATH_SRC)/vector.o \
	$(PATH_SRC)/zstring_buffer.o \
	$(PATH_SRC)/zstring.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
