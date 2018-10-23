CXX ?= g++

# path #
SRC_PATH = $(shell pwd)
BUILD_PATH = Build
BIN_PATH = Application

# executable #
BIN_NAME = MyGame

# extensions #
SRC_EXT = cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -std=c++14 -Wall -Wextra -g
INCLUDES = -I $(SRC_PATH)/TrikytaEngine/libs/includes/ -I $(SRC_PATH)/TrikytaEngine/src/ -I $(SRC_PATH)/PhysicsEngine/ -I $(SRC_PATH)/PhysicsEngine/PhysicsEngine/Core/ -I $(SRC_PATH)/Libraries/src/ -I $(SRC_PATH)/Libraries/src/TinyXML -I $(SRC_PATH)/Libraries/src/zlib -I $(SRC_PATH)/LuaBinds/src/ -I $(SRC_PATH)/LuaBinds/lua/include/ -I $(SRC_PATH)/Game/src/
# Space-separated pkg-config libraries used by this project
LIBS = `sdl2-config --libs --cflags` -LLinuxLibs/ -lSDL2 -LLinuxLibs/ -lSDL2_image -LLinuxLibs/ -lSDL2_ttf -LLinuxLibs/ -llua -LLinuxLibs/ -lz -lm
#`sdl2-config --libs --cflags` -LLinuxLibs/ -lSDL2main -LLinuxLibs/ -lSDL_ttf -LLinuxLibs/ -lSDL2_image
.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
