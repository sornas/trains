GAME = banverket
FOG_DIR = fog
LIB_DIR = lib
INC_DIR = inc

CC = gcc
CXX = g++

WARNINGS = -Wall
FLAGS = $(WARNINGS) -std=c11
DEBUG_FLAGS = $(FLAGS) -ggdb -O0

ARCH = $(shell uname -s | cut -c -5)
ifeq ($(TARGET),WINDOWS)
	ARCH = MINGW
endif

ENGINE = libfog.a
ifeq ($(ARCH),MINGW)
	# Cross compilation
	ENGINE = libfog.lib
	CC = x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++
endif
ENGINE_PATH = $(LIB_DIR)/$(ENGINE)

LIBS = -lfog -lSDL2 -lSDL2main -lpthread
ifneq ($(ARCH),MINGW)
	LIBS += -lc -ldl -lm
endif
INCLUDES = -I$(INC_DIR)

ASSET_BUILDER = $(FOG_DIR)/out/mist
ASSET_FILE = data.fog
ASSETS = $(shell find res/ -type f -name "*.*")

HEADERS = $(shell find src/ -type f -name "*.h")
SOURCES = $(shell find src/ -type f -name "*.c")
OBJECTS = $(SOURCES:src/%.c=%.o)
CONFIG = src/config.h

default: all

all: game

game: $(GAME)
$(GAME): $(ENGINE_PATH) $(OBJECTS) $(ASSET_FILE)
	$(CC) $(DEBUG_FLAGS) $(OBJECTS) -o $@ -L$(LIB_DIR) $(LIBS)

.PHONY: run
run: $(GAME)
	./$(GAME)

.PHONY: debug
debug: $(GAME)
	gdb -ex "b _fog_illegal_allocation" ./$(GAME)

%o: src/%c $(HEADERS) $(CONFIG)
	$(CC) $(DEBUG_FLAGS) -c $< -o $@ $(INCLUDES) -include $(CONFIG)

.NOTPARALLEL: $(ENGINE_PATH)
$(ENGINE_PATH): | $(LIB_DIR) $(INC_DIR)
	make -C $(FOG_DIR) engine ENGINE_LIBRARY_NAME=$(ENGINE) CXX=$(CXX)
	make -C $(FOG_DIR) rain mist
	cp $(FOG_DIR)/out/$(ENGINE) $(LIB_DIR)/
	cp $(FOG_DIR)/out/fog.h $(INC_DIR)/

$(ASSET_FILE): $(ASSETS) $(ASSET_BUILDER)
	$(ASSET_BUILDER) -o $@ $(ASSETS)

.PHONY: clean
clean:
	rm -f $(GAME)
	rm -f $(GAME).exe
	rm -f $(ASSET_FILE)
	rm -f *.o

.PHONY: clean-all
clean-all: clean
	make -C $(FOG_DIR) clean
	rm -rf $(LIB_DIR)

#-----------------
$(LIB_DIR):
	mkdir -p $@

$(INC_DIR):
	mkdir -p $@
