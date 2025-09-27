CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c17

ifeq ($(OS),Windows_NT)      # Windows
    RM = del /F /Q
    EXT = .exe
else                         # Linux / macOS
    RM = rm -f
    EXT =
endif

TARGETS = graham_scan$(EXT) jarvis_march$(EXT) andrew_monotone_chain$(EXT) chan_algorithm$(EXT)

all: $(TARGETS)

graham_scan$(EXT): graham_scan.c
	$(CC) $(CFLAGS) $< -o $@

jarvis_march$(EXT): jarvis_march.c
	$(CC) $(CFLAGS) $< -o $@

andrew_monotone_chain$(EXT): andrew_monotone_chain.c
	$(CC) $(CFLAGS) $< -o $@

chan_algorithm$(EXT): chain_algorithm.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-$(RM) $(TARGETS)
