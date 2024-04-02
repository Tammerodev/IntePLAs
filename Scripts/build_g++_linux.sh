#!/bin/bash

# Script to build project
cd src

g++ -O3 -Wall -Wno-unused-function -Wno-unused-variable *.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network -ltgui -ljsoncpp -o ./bin/Build