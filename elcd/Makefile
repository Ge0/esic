SOLUTION_DIR  = ..

PATH_SRC      = ./src
PATH_INCLUDES = $(SOLUTION_DIR)/includes

CC = gcc -m32 -I$(PATH_INCLUDES) -DEMULATOR

# lcd.c  lcd_emulator.c  lcd_painter.c
OBJ =	$(PATH_SRC)/lcd.o \
	$(PATH_SRC)/lcd_painter.o \

all: $(OBJ)


.o: .c
	$(CC) -c $^

clean:
	@rm -f $(PATH_SRC)/*.o *.o
