GAME = banverket
FOG_FOLDER = fog

CC_GCC = gcc
CXX_GCC = g++
CC_MINGW = x86_64-w64-mingw32-gcc
CXX_MINGW = x86_64-w64-mingw32-g++

WARNINGS = -Werror -Wall
FLAGS = $(WARNINGS) -std=c11
DEBUG_FLAGS = $(FLAGS) -ggdb -O0
LIB_FOLDER = lib
NATIVE5 = $(shell uname -s | cut -c -5)
ARCH5 = $(NATIVE5)

ifeq ($(TARGET),WINDOWS)
	ARCH5 = MINGW
endif

ENGINE =
CC = 
CXX = 
#ifeq ($(ARCH5),Darwi)
#	ENGINE = libfog.dylib
#	CC = CC_GCC
#	CXX = CXX_GCC
#endif
ifeq ($(ARCH5),Linux)
	ENGINE = libfog.a
	CC = $(CC_GCC)
	CXX = $(CXX_GCC)
endif
ifeq ($(ARCH5),MINGW)
	ENGINE = libfog.lib
	CC = $(CC_MINGW)
	CXX = $(CXX_MINGW)
endif

LIBS = -lfog -lSDL2 -lSDL2main -lpthread

ifneq ($(ARCH5),MINGW)
	LIBS += -lm -ldl -lc
endif
ifeq ($(ARCH5),Darwin)
	LIBS += -lc++
endif

INCLUDES = -Iinc

ASSET_BUILDER = $(FOG_FOLDER)/out/mist
ASSET_FILE = data.fog
ASSETS = $(shell find res -type f -name "*.*")
HEADERS = $(shell find src -type f -name "*.h")
SRCS = $(shell find src -type f -name "*.c")
OBJS = $(SRCS:src/%.c=%.o)
CONFIG = src/config.h

.PHONY: default run game engine asset update-engine clean clean-all clean-engine clean-game $(LIB_FOLDER)/$(ENGINE) all debug

default: game
all: clean update-engine run
game: $(GAME)
engine: $(LIB_FOLDER)/$(ENGINE)
asset: $(ASSET_FILE)

debug: game
	gdb $(GAME)

info:
	$(info $(ARCH5))
	$(info $(SRCS))
	$(info $(OBJS))

run: game
	./$(GAME)

$(ASSET_FILE): $(ASSETS) $(ASSET_BUILDER)
	@$(ASSET_BUILDER) -o $@ $(ASSETS)

$(GAME): $(LIB_FOLDER)/$(ENGINE) $(OBJS) $(ASSET_FILE)
	$(CC) $(DEBUG_FLAGS) $(OBJS) -o $@ -L$(LIB_FOLDER) $(LIBS)

%.o: src/%.c $(HEADERS) $(CONFIG)
	$(CC) $(DEBUG_FLAGS) -c $< -o $@ $(INCLUDES) -include $(CONFIG)

$(ASSET_BUILDER): $(LIB_FOLDER)/$(ENGINE)

$(LIB_FOLDER):
	@mkdir -p $@

update-engine:
	@git submodule update --remote

.NOTPARALLEL: $(LIB_FOLDER)/$(ENGINE)
$(LIB_FOLDER)/$(ENGINE): | $(LIB_FOLDER)
	make -C $(FOG_FOLDER) engine ARCH5=$(ARCH5) ENGINE_PROGRAM_NAME=$(ENGINE) CXX=$(CXX)
	make -C $(FOG_FOLDER) rain mist
	@cp $(FOG_FOLDER)/out/$(ENGINE) $(LIB_FOLDER)/
	@mkdir -p inc
	@cp $(FOG_FOLDER)/out/fog.h inc/

clean: clean-game

clean-all: clean-engine clean-game

clean-game:
	rm -f $(GAME)
	rm -f $(GAME).exe
	rm -f $(ASSET_FILE)
	rm -f *.o

clean-engine:
	make -C $(FOG_FOLDER) clean
	rm -rf $(LIB_FOLDER)


gdb: $(GAME)
	gdb -ex "b _fog_illegal_allocation()" ./$(GAME)
