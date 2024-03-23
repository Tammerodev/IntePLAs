#!/bin/bash

cd ..                                   # Now in project root
cd ..                                   # Now in project root location

mkdir BuildOutput                       # Folder for output

cp -a inteplas/res BuildOutput          # Copy all assets to BuildOutput
cp -a inteplas/json BuildOutput          # Copy JSON data to BuildOutput
cp -a inteplas/src/main BuildOutput     # Copy executable to BuildOutput

