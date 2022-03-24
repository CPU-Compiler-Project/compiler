GRM=compiler.y
LEX=compiler.l
BIN=compiler

CC=gcc
CFLAGS=-Wall -g

BUILD_DIR=build
SOURCE_DIR=src

BIN_DIR=$(BUILD_DIR)/bin

OBJ=y.tab.o lex.yy.o functions.o

all: $(BIN_DIR) $(BIN_DIR)/$(BIN)
	
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/y.tab.c: $(SOURCE_DIR)/$(GRM)
	yacc -d $< -o $@

$(BUILD_DIR)/lex.yy.c: $(SOURCE_DIR)/$(LEX)
	flex -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(BIN): $(addprefix $(BUILD_DIR)/, $(OBJ))
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	$(RM) -r $(BUILD_DIR)/*

