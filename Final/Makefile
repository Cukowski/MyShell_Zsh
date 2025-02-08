TARGET = my_shell
SRC_DIR = src
OBJ = $(SRC_DIR)/main.c $(SRC_DIR)/input_parser.c $(SRC_DIR)/helpers.c $(SRC_DIR)/builtins.c $(SRC_DIR)/executor.c
CFLAGS = -Wall -Wextra -Werror
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(SRC_DIR)/*.o

fclean: clean
	rm -f $(TARGET)

re: fclean all
