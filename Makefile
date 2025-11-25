# ============================================================
# Makefile — Sistema de Controle de Pedidos (SCP)
# ============================================================

CC       := gcc
CFLAGS   := -Wall -Wextra -Iinclude
LDLIBS   := -lncurses

BUILD_DIR := build
SRC_DIR   := src
INC_DIR   := include

EXEC := $(BUILD_DIR)/scp

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all clean run help dirs

all: dirs $(EXEC)
	@echo "Compilação concluída com sucesso!"

dirs:
	@mkdir -p $(BUILD_DIR)

$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Limpeza concluída."

run: all
	@echo "Executando o Sistema Controle de Pedidos..."
	@$(EXEC)

help:
	@echo "Comandos disponíveis:"
	@echo "  make       → compila o projeto"
	@echo "  make run   → compila e executa"
	@echo "  make clean → remove arquivos compilados"
	@echo "  make help  → mostra esta ajuda"
