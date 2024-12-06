SRC_GAME_DIR := src/game
HEADERS_GAME_DIR := include
SRC_TEST_DIR := test

SRC_GAME := \
	    $(shell find $(SRC_GAME_DIR)/ -name "*.cpp") \
	    external/cJSON/src/cJSON.c
HEADERS_GAME := $(shell find $(HEADERS_GAME_DIR)/ -name "*.hpp")
SRC_TEST := \
	    $(filter-out $(SRC_GAME_DIR)/main.cpp, $(SRC_GAME)) \
	    $(shell find $(SRC_TEST_DIR)/ -name "*.cpp")

SRC_SERVER_ROUTE := src/server_route/main.cpp
SRC_SERVER_ROUTE_TEST_CLIENT := src/server_route/test_client.cpp

CC := g++
CC_MINGW := x86_64-w64-mingw32-g++
INCLUDES := -Iinclude -Iexternal -Iexternal/cJSON/include/
INCLUDES_TEST := -I/opt/gtest/include/
COMMON_COMPILER_FLAGS := $(INCLUDES) -Wall -Wextra -Wpedantic -std=c++17
COMPILER_FLAGS_GAME := \
		       $(COMMON_COMPILER_FLAGS) \
		       -DGRAPHICS_ENABLED=1
COMPILER_FLAGS_GAME_WINDOWS := \
			   -I/mnt/d/mingw_dev/sdl_combined_headers/include \
		       $(COMMON_COMPILER_FLAGS) \
		       -static-libgcc \
		       -static-libstdc++ \
		       -DGRAPHICS_ENABLED=1
COMPILER_FLAGS_TEST := \
		      $(INCLUDES_TEST) \
		      $(COMMON_COMPILER_FLAGS) \
		      -pthread \
		      -DBUILD_FOR_TESTS=1
COMPILER_FLAGS_SERVER_ROUTE := \
			       $(COMMON_COMPILER_FLAGS)
COMPILER_FLAGS_SERVER_ROUTE_TEST_CLIENT := $(COMPILER_FLAGS_SERVER_ROUTE)

# Flag -static is not used, as Windows would not have at runtime the shared
# libraries from the glibc version used for linking.
LINKER_FLAGS = -lSDL2 -lSDL2_image

LINKER_FLAGS_WINDOWS := \
			-L/mnt/d/mingw_dev/SDL2-2.30.6/x86_64-w64-mingw32/lib/ \
			-L/mnt/d/mingw_dev/SDL2_image-2.8.2/x86_64-w64-mingw32/lib/ \
			-Wl,-subsystem,windows \
			-lmingw32 \
			-lSDL2main \
			-lSDL2 \
			-lSDL2_image
LINKER_FLAGS_TEST := \
		    $(LINKER_FLAGS) \
		    -lgtest \
		    -L/opt/gtest/lib
EXE_NAME_GAME := rogue_forever
EXE_NAME_GAME_DEBUG := rogue_forever_debug
WINDOWS_EXE_NAME_GAME := rogue_forever.exe
EXE_NAME_SERVER_ROUTE := rogue_forever_server_route
EXE_NAME_SERVER_ROUTE_TEST_CLIENT := rogue_forever_server_route_test_client
EXE_NAME_TEST := rogue_forever_test
WINDOWS_DEPLOY_DST := /mnt/d/rogue_forever_deploy
WINDOWS_DEPLOY_SRC := \
			./assets \
			./maps \
			$(WINDOWS_EXE_NAME_GAME) \
			./SDL2.dll \
			./SDL2_image.dll

ALL_EXE_NAMES := \
		 $(EXE_NAME_GAME) \
		 $(EXE_NAME_TEST)

ALL_WINDOWS_EXE_NAMES := \
		 $(WINDOWS_EXE_NAME_GAME)

linux: $(EXE_NAME_GAME)

linux_debug: $(EXE_NAME_GAME_DEBUG)

linux_run: $(EXE_NAME_GAME)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_GAME)

windows: $(ALL_WINDOWS_EXE_NAMES)

all: linux windows

windows_deploy: windows
	if [ ! -d "$(WINDOWS_DEPLOY_DST)" ] ; then mkdir $(WINDOWS_DEPLOY_DST) ; fi
	cp -r $(WINDOWS_DEPLOY_SRC) $(WINDOWS_DEPLOY_DST)

$(EXE_NAME_GAME): $(SRC_GAME) $(HEADERS_GAME_DIR)
	$(CXX) $(SRC_GAME) $(COMPILER_FLAGS_GAME) $(LINKER_FLAGS) -o $@

$(EXE_NAME_GAME_DEBUG): $(SRC_GAME) $(HEADERS_GAME_DIR)
	$(CXX) $(SRC_GAME) $(COMPILER_FLAGS_GAME) $(LINKER_FLAGS) -g -o $@

$(WINDOWS_EXE_NAME_GAME): $(SRC_GAME) $(HEADERS_GAME_DIR)
	$(CC_MINGW) $(SRC_GAME) $(COMPILER_FLAGS_GAME_WINDOWS) $(LINKER_FLAGS_WINDOWS) -o $@

$(EXE_NAME_SERVER_ROUTE): $(SRC_SERVER_ROUTE)
	$(CXX) $(SRC_SERVER_ROUTE) $(COMPILER_FLAGS_SERVER_ROUTE) -o $@

$(EXE_NAME_SERVER_ROUTE_TEST_CLIENT): $(SRC_SERVER_ROUTE_TEST_CLIENT)
	$(CXX) $(SRC_SERVER_ROUTE_TEST_CLIENT) $(COMPILER_FLAGS_SERVER_ROUTE_TEST_CLIENT) -o $@

$(EXE_NAME_TEST): $(SRC_TEST)
	$(CXX) $^ $(COMPILER_FLAGS_TEST) $(LINKER_FLAGS_TEST) -o $@

# TODO fix and print all actually available targets. Note that ALL_EXE_NAMES becomes
# unused then.
targets:
	@echo $(ALL_EXE_NAMES)

test: $(EXE_NAME_TEST)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_TEST)
