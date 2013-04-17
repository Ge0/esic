SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

# xmlparse.c  xmlrole.c  xmltok.c  xmltok_impl.c  xmltok_ns.c

CC = gcc -m32 -I$(PATH_INCLUDES) -I$(PATH_INCLUDES)/expat -DEMULATOR

OBJ =	$(PATH_SRC)/xmlparse.o \
	$(PATH_SRC)/xmlrole.o \
	$(PATH_SRC)/xmltok.o \
	$(PATH_SRC)/xmltok_impl.o \
	$(PATH_SRC)/xmltok_ns.o

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
