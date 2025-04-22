DEV_BUILD_NAME := Dev
DEBUG_BUILD_NAME := Debug
RELEASE_BUILD_NAME := Release
BUILD_DIR_PREFIX := build
DEV_BUILD_DIR := $(BUILD_DIR_PREFIX)-Dev
DEBUG_BUILD_DIR := $(BUILD_DIR_PREFIX)-$(DEBUG_BUILD_NAME)
RELEASE_BUILD_DIR := $(BUILD_DIR_PREFIX)

dev:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(DEV_BUILD_NAME) -S . -B $(DEV_BUILD_DIR) && cmake --build $(DEV_BUILD_DIR) --config $(DEV_BUILD_NAME)

debug:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(DEBUG_BUILD_NAME) -S . -B $(DEBUG_BUILD_DIR) && cmake --build $(DEBUG_BUILD_DIR) --config $(DEBUG_BUILD_NAME)

release:
	cmake -DCMAKE_BUILD_TYPE:STRING=$(RELEASE_BUILD_NAME) -S . -B $(RELEASE_BUILD_DIR) && cmake --build $(RELEASE_BUILD_DIR) --config $(RELEASE_BUILD_NAME)

check:
	find . -maxdepth 1 -type d -name 'build*' -exec run-clang-tidy -config-file='.clang-tidy' -p {} src/** ';' -quit

clean:
	rm -rf $(DEV_BUILD_DIR) $(DEBUG_BUILD_DIR) $(RELEASE_BUILD_DIR)
