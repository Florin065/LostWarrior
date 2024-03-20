TARGET = game

CC     = g++
INC    = -IInclude -ISource
LIBS   = -lGL -lglfw -lassimp
CFLAGS = -std=c++23 -Wall -Wextra -pedantic

SOURCE_DIR = Source
BUILD_DIR  = Build
TARGET_DIR = Bin

SOURCES = $(shell find $(SOURCE_DIR) -name "*.cpp")
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
OBJDIRS = $(dir $(OBJECTS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $(TARGET_DIR)/$@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rf $(TARGET_DIR) $(BUILD_DIR)

