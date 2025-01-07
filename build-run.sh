#!/usr/bin/env bash
mkdir -p build
#below is for full debugging
#g++ -g -O0 -Wall -Wextra src/*.cpp -o build/opengl -lSDL2 -lGLEW -lGL
g++ -g -O src/*.cpp -o build/opengl -lSDL2 -lGLEW -lGL
./build/opengl
