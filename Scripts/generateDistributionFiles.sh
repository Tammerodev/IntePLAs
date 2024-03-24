#!/bin/bash

cd ..                                    # Now in project root

ls -a

mkdir BuildOutput                        # Folder for output
mkdir BuildOutput/bin

cp -a inteplas/res BuildOutput           # Copy all assets to BuildOutput
cp -a inteplas/json BuildOutput          # Copy JSON data to BuildOutput

cp -a inteplas/src/main BuildOutput/bin  # Copy executable to BuildOutput

zip -r build.zip BuildOutput

rm -r BuildOutput