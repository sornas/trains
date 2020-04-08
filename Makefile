GAME = banverket
FOG_FOLDER = fog

CC = gcc
WARNINGS = -Werror -Wall
FLAGS = $(WARNINGS) -std=c11
DEBUG_FLAGS = $(FLAGS) -ggdb -O0
LIB_FOLDER = lib
ARCH = $(shell uname -s)

ENGINE =
ifeq ($(ARCH),Darwin)
	ENGINE = $(LIB_FOLDER)/libfog.dylib
endif
ifeq ($(ARCH),Linux)
	ENGINE = $(LIB_FOLDER)/libfog.a
endif

# Would be nice to remove some of these...
LIBS = -lfog -lSDL2 -lSDL2main -ldl -lpthread -lc -lm
ifeq ($(ARCH),Darwin)
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

.PHONY: default run game engine asset update-engine clean clean-all clean-engine clean-game $(ENGINE) all debug

default: game
all: clean update-engine run
game: $(GAME)
engine: $(ENGINE)
asset: $(ASSET_FILE)

debug: game
	gdb $(GAME)

info:
	$(info $(SRCS))
	$(info $(OBJS))

run: game
	./$(GAME)

$(ASSET_FILE): $(ASSETS) $(ASSET_BUILDER)
	@$(ASSET_BUILDER) -o $@ $(ASSETS)

$(GAME): $(ENGINE) $(OBJS) $(ASSET_FILE)
	$(CC) $(DEBUG_FLAGS) $(OBJS) -o $@ -L$(LIB_FOLDER) $(LIBS)

%.o: src/%.c $(HEADERS) $(CONFIG)
	$(CC) $(DEBUG_FLAGS) -c $< -o $@ $(INCLUDES) -include $(CONFIG)

$(ASSET_BUILDER): $(ENGINE)

$(LIB_FOLDER):
	@mkdir -p $@

update-engine:
	@git submodule update --remote

.NOTPARALLEL: $(ENGINE)
$(ENGINE): | $(LIB_FOLDER)
	make -C $(FOG_FOLDER) engine
	@cp $(FOG_FOLDER)/out/libfog.* $(LIB_FOLDER)/
	@mkdir -p inc
	@cp $(FOG_FOLDER)/out/fog.h inc/

clean: clean-game

clean-all: clean-engine clean-game

clean-game:
	rm -f $(GAME)
	rm -f $(ASSET_FILE)
	rm -f *.o

clean-engine:
	make -C $(FOG_FOLDER) clean
	rm -rf $(LIB_FOLDER)


gdb: $(GAME)
	gdb ./$(GAME)
