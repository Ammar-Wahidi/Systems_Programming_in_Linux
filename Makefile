# Makefile for Unix Shell Implementations
# Author: Systems Programming in Linux Course
# Instructor: Reda Maher

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SHELLS = femtoshell picoshell nanoshell microshell
UTILS = cp mv echo pwd

# Default target: build everything
all: $(SHELLS) $(UTILS)
	@echo "All programs compiled successfully!"

# Shell targets
femtoshell: Femto\ Shell/FemtoShell.c
	$(CC) $(CFLAGS) -o $@ "Femto Shell/FemtoShell.c"
	@echo "FemtoShell compiled successfully!"

picoshell: Pico\ Shell/Pico_Shell.c
	$(CC) $(CFLAGS) -o $@ "Pico Shell/Pico_Shell.c"
	@echo "PicoShell compiled successfully!"

nanoshell: Nano\ Shell/Nano_Shell.c
	$(CC) $(CFLAGS) -o $@ "Nano Shell/Nano_Shell.c"
	@echo "NanoShell compiled successfully!"

microshell: Micro\ Shell/Micro_Shell.c
	$(CC) $(CFLAGS) -o $@ "Micro Shell/Micro_Shell.c"
	@echo "MicroShell compiled successfully!"

# Utility targets
cp: Unix\ Utility/cp.c
	$(CC) $(CFLAGS) -o $@ "Unix Utility/cp.c"
	@echo "cp utility compiled successfully!"

mv: Unix\ Utility/mv.c
	$(CC) $(CFLAGS) -o $@ "Unix Utility/mv.c"
	@echo "mv utility compiled successfully!"

echo: Unix\ Utility/echo.c
	$(CC) $(CFLAGS) -o $@ "Unix Utility/echo.c"
	@echo "echo utility compiled successfully!"

pwd: Unix\ Utility/pwd.c
	$(CC) $(CFLAGS) -o $@ "Unix Utility/pwd.c"
	@echo "pwd utility compiled successfully!"

# Build only shells
shells: $(SHELLS)
	@echo "All shells compiled successfully!"

# Build only utilities
utils: $(UTILS)
	@echo "All utilities compiled successfully!"

# Debug builds
debug: CFLAGS += -g -DDEBUG
debug: clean all
	@echo "Debug builds completed!"

# Clean up compiled binaries
clean:
	rm -f $(SHELLS) $(UTILS)
	@echo "Cleaned up all binaries!"

# Run tests (you can add your test commands here)
test: all
	@echo "Running basic tests..."
	@./femtoshell < /dev/null || true
	@echo "Test completed!"

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Compile all shells and utilities (default)"
	@echo "  shells     - Compile only shells"
	@echo "  utils      - Compile only utilities"
	@echo "  debug      - Compile with debug symbols"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  test       - Run basic tests"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Individual targets:"
	@echo "  femtoshell - Compile FemtoShell"
	@echo "  picoshell  - Compile PicoShell"
	@echo "  nanoshell  - Compile NanoShell"
	@echo "  microshell - Compile MicroShell"
	@echo "  cp         - Compile cp utility"
	@echo "  mv         - Compile mv utility"
	@echo "  echo       - Compile echo utility"
	@echo "  pwd        - Compile pwd utility"

.PHONY: all shells utils debug clean test help
