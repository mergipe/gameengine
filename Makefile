RELWITHDEBINFO_BUILD_NAME := RelWithDebInfo
DEBUG_BUILD_NAME := Debug
RELEASE_BUILD_NAME := Release
BUILD_DIR_PREFIX := build-
RELWITHDEBUGINFO_BUILD_DIR := $(BUILD_DIR_PREFIX)$(RELWITHDEBINFO_BUILD_NAME)
DEBUG_BUILD_DIR := $(BUILD_DIR_PREFIX)$(DEBUG_BUILD_NAME)
RELEASE_BUILD_DIR := $(BUILD_DIR_PREFIX)$(RELEASE_BUILD_NAME)

relwithdebinfo:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(RELWITHDEBINFO_BUILD_NAME) -S . -B $(RELWITHDEBUGINFO_BUILD_DIR) && cmake --build $(RELWITHDEBUGINFO_BUILD_DIR)

debug:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(DEBUG_BUILD_NAME) -S . -B $(DEBUG_BUILD_DIR) && cmake --build $(DEBUG_BUILD_DIR)

release:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(RELEASE_BUILD_NAME) -S . -B $(RELEASE_BUILD_DIR) && cmake --build $(RELEASE_BUILD_DIR)