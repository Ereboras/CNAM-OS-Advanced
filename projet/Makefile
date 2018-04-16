CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
BIN_DIR=./bin
HEADERS_DIR=./headers
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)

EXEC=prog

GEXEC=$(EXEC).cov

all: $(SRC) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ $(SRC)

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	# generate some data for gcov by calling the generated binary with various options
	$(GCOV_DIR)/$(GEXEC) -h
	$(GCOV_DIR)/$(GEXEC) -i input -o output -v

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(GEXEC)
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

clean:
	rm -rf $(SRC_DIR)/*.o
	rm tmp_command

mrproper: clean
	rm -rf $(BIN_DIR)/*

.PHONY: clean doc