CXX = clang
CXX_ARGS = -std=c++2a -I include

# Directory Constants
SRC = etern
BIN = bin/etern

.PHONY: all clean

SRCS = $(shell find $(SRC) -name "*.cpp")

all:
	$(CXX) $(CXX_ARGS) -o $(BIN) $(SRCS)

clean:
	rm bin/etern