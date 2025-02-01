CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -pedantic -Ofast -Werror=ignored-qualifiers -g `pkg-config --cflags glib-2.0`
LDFLAGS := `pkg-config --libs glib-2.0`

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := .
RECOMENDADOR_DIR := recomendador

SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC)) $(BUILD_DIR)/recomendador.o

TARGET_MAIN := $(BIN_DIR)/programa-principal
TARGET_TEST := $(BIN_DIR)/programa-testes
TARGET_INTERACTIVE := $(BIN_DIR)/programa-interativo

MAIN_OBJ := $(BUILD_DIR)/main.o
TEST_OBJ := $(BUILD_DIR)/programa-testes.o
INTERACTIVE_OBJ := $(BUILD_DIR)/programa-interativo.o
COMMON_OBJ := $(filter-out $(MAIN_OBJ) $(TEST_OBJ) $(INTERACTIVE_OBJ), $(OBJ))

all: create_dirs $(TARGET_MAIN) $(TARGET_TEST) $(TARGET_INTERACTIVE)

create_dirs:
	@mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/recomendador.o: $(RECOMENDADOR_DIR)/recomendador-linux-x86_64.o
	@mkdir -p $(@D)
	cp $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_MAIN): $(MAIN_OBJ) $(COMMON_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET_TEST): $(TEST_OBJ) $(COMMON_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET_INTERACTIVE): $(INTERACTIVE_OBJ) $(COMMON_OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(TARGET_MAIN) $(TARGET_TEST) $(TARGET_INTERACTIVE) $(BUILD_DIR) resultados/*.csv resultados/*.txt

rebuild: clean all 

.PHONY: clean rebuild