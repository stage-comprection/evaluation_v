# Compiler options
CC = g++
OPTCFLAGS = -Ofast -march=native 
CFLAGS = -Wall -std=c++11 $(OPTCFLAGS)
LDFLAGS = -pthread -static-libstdc++

# Directory organisation
BASEDIR = .
BIN = $(BASEDIR)/bin
SRC = $(BASEDIR)/src
BUILD = $(BASEDIR)/build

# Module MAIN
SRC_MAIN = $(SRC)/main
BUILD_MAIN = $(BUILD)/main
CPP_MAIN = $(wildcard $(SRC_MAIN)/*.cpp)

# Module MAPPING
SRC_MAPPING = $(SRC)/mapping
BUILD_MAPPING = $(BUILD)/mapping
CPP_MAPPING = $(wildcard $(SRC_MAPPING)/*.cpp)

# Target
TARGET = evaluation

# Variables
OBJS = $(addprefix $(BUILD_MAIN)/, $(notdir $(CPP_MAIN:.cpp=.o))) $(addprefix $(BUILD_MAPPING)/, $(notdir $(CPP_MAPPING:.cpp=.o)))

# Rules

all: init print-OBJS $(TARGET)

print-%  : ; @echo $* = $($*)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$(TARGET) $^

$(BUILD_MAIN)/%.o: $(SRC_MAIN)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

$(BUILD_MAPPING)/%.o: $(SRC_MAPPING)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)/*.o $(BUILD_MAIN)/*.o $(BUILD_MAPPING)/*.o
	rm -rf $(BIN)/$(TARGET)

init: 
	mkdir -p $(BUILD) $(BUILD_MAPPING) $(BUILD_MAIN) $(BIN)

rebuild: clean $(TARGET)
