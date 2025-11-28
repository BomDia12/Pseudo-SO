# ====================================
# Makefile com arquivos definidos manualmente
# ====================================

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -std=c11 -g

# ---------- LISTA MANUAL DOS ARQUIVOS .C ----------

# Coloque aqui SOMENTE os arquivos que você quer compilar
SRCS = \
    main.c \
    disk_reader.c 

# Gera .o manualmente a partir dos .c definidos acima
OBJS = $(SRCS:.c=.o)

# Nome do executável final
TARGET = program

# ---------- Regras de compilação ----------

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---------- Execução ----------
# Exemplo: make run FILE=input.txt
run: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
	    echo "Uso: make run FILE=arquivo.txt"; \
	    exit 1; \
	fi
	./$(TARGET) $(FILE)

# ---------- Limpeza ----------
clean:
	rm -f $(OBJS) $(TARGET)

distclean: clean
	rm -rf cppcheck_report

# ---------- Cppcheck ----------
cppcheck:
	mkdir -p cppcheck_report
	cppcheck --enable=all --inconclusive --std=c11 --quiet \
	         --suppress=missingIncludeSystem \
	         --xml --xml-version=2 . \
	         2> cppcheck_report/report.xml
	@echo "Relatório gerado em: cppcheck_report/report.xml"

.PHONY: all clean distclean cppcheck run
