# Script to build project

#!/bin/bash

cd ..
cd src

g++ -g -O2 *.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network -ltgui -ljsoncpp -o Build