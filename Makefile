CC ?= cc
PKG_CONFIG ?= pkg-config
CFLAGS ?= -std=c99 -O2 -Wall -Wextra
RAYLIB_CFLAGS = $(shell $(PKG_CONFIG) --cflags raylib)
RAYLIB_LIBS = $(shell $(PKG_CONFIG) --libs raylib)

.PHONY: all run clean check-deps

all: build/spacegame

check-deps:
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo "Install pkg-config to build SpaceGame." >&2; exit 1; }
	@$(PKG_CONFIG) --exists raylib || { echo "raylib was not found. See the setup instructions in README.md." >&2; exit 1; }

build/spacegame: main.c Makefile | check-deps
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(RAYLIB_CFLAGS) main.c -o $@ $(LDFLAGS) $(RAYLIB_LIBS) $(LDLIBS) -lm

run: build/spacegame
	./build/spacegame

clean:
	$(RM) build/spacegame
