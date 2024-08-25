CXX = g++
C = gcc
CXX_ARGS = -std=c++2a -I ./include -I ./lib

# Directory Constants
SRC = etern
BIN = bin/etern

.PHONY: all clean debug

SRCS = $(shell find $(SRC) -name "*.cpp")

all:
	$(CXX) $(CXX_ARGS) -o $(BIN) $(SRCS) ./lib/json/yyjson.c

debug:
	$(CXX) $(CXX_ARGS) -g -o $(BIN) $(SRCS) ./lib/json/yyjson.c

clean:
	rm bin/etern