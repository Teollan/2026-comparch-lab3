.PHONY: build run test lint format clean

CXX ?= g++
CC  ?= gcc

build:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC)
	cmake --build build

run: build
	./build/app

test: build
	ctest --test-dir build --output-on-failure

SOURCES := main.cpp $(shell find src -name '*.cpp' -o -name '*.hpp' -o -name '*.tpp' 2>/dev/null)

format:
	clang-format -i $(SOURCES)

lint:
	cmake -B build -DENABLE_CLANG_TIDY=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC)
	cmake --build build --clean-first

clean:
	rm -rf build
