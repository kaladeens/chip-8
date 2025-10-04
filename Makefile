CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I$(CURDIR)/local/include/SDL2
LDFLAGS := -L$(CURDIR)/local/lib -lSDL2

SRC := src/main.cpp
BIN := chip8

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN)
	# ensure loader can find the shared lib at runtime (Linux/macOS)
	LD_LIBRARY_PATH=$(CURDIR)/local/lib DYLD_LIBRARY_PATH=$(CURDIR)/local/lib ./$(BIN)

clean:
	rm -f $(BIN)
