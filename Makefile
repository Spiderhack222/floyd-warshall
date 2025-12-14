CC = gcc
CFLAGS = -Wall -O2
SRC = src/*.c
BIN = bin/floyd
TESTS = tests/test1.txt tests/test2.txt tests/test3.txt

.PHONY: all run test clean pdf

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

run: $(BIN)
	@echo "Ejecutando bin/floyd (usa argumento opcional de archivo)"
	@./$(BIN)

test: $(BIN)
	@echo "Ejecutando tests y guardando salidas en tests/output_test*.txt"
	@./$(BIN) tests/test1.txt > tests/output_test1.txt || true
	@./$(BIN) tests/test2.txt > tests/output_test2.txt || true
	@./$(BIN) tests/test3.txt > tests/output_test3.txt || true
	@echo "Listo."

pdf:
	@echo "Intentando convertir REPORT.md a REPORT.pdf con pandoc..."
	@pandoc REPORT.md -o REPORT.pdf || { echo "pandoc no encontrado. Instala pandoc y ejecuta 'make pdf' de nuevo."; exit 1; }

package: all test pdf
	@echo "Creando paquete deliverable.zip con src, tests, README.md y REPORT.pdf"
	@zip -r deliverable.zip src tests README.md REPORT.md REPORT.pdf Makefile || true

clean:
	rm -f $(BIN) REPORT.pdf
	rm -rf bin
