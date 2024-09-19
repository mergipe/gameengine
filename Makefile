TARGET_EXEC := gameengine
BUILD_DIR := ./build
TARGET_PATH := $(BUILD_DIR)/$(TARGET_EXEC)
SRC_DIR := ./src
LIBS_DIR := ./libs
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
CXX := g++
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4
DEBUG_FLAGS := -ggdb
RELEASE_FLAGS := -O3 -DNDEBUG
SRC_OBS := $(SRC_FILES:%=$(BUILD_DIR)/%.o)
INC_DIRS := $(shell find $(SRC_DIR) -type d) $(LIBS_DIR)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) $(addprefix -isystem,$(INC_DIRS))
CXXFLAGS := -Wall -std=c++23 $(INC_FLAGS)
SRC_CXXFLAGS := $(CXXFLAGS)
SRC_CXXFLAGS += -pedantic-errors -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -xc++
SRC_CXXFLAGS += `sdl2-config --cflags` -I$(LIBS_DIR)

release: SRC_CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET_PATH)

debug: SRC_CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET_PATH)

$(TARGET_PATH): $(SRC_OBS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(SRC_CXXFLAGS) -c $< -o $@

run:
	./$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
