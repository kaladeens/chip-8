CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinc -Isrc -Ilib/SDL3-3.2.24/include
LDFLAGS := -Llib/SDL3-3.2.24/build -lSDL3

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=build/%.o)
TARGET := emu

all: $(TARGET)

# Link the final binary
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile object files
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

.PHONY: all clean
