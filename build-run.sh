#!/usr/bin/env bash
mkdir -p build
g++ src/*.cpp -o build/opengl -lSDL2 -lGLEW -lGL
./build/opengl