CC=gcc

SRC_DIR=./src
BIN_DIR=./bin
HEADERS_DIR=./headers

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)

EXEC=prog

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ $(SRC)

clean:
	rm -rf $(SRC_DIR)/*.o

mrproper: clean
	rm -rf $(BIN_DIR)/*

.PHONY: clean