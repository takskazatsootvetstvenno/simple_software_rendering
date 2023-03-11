#!/bin/bash

if [ -e build ]
then
	echo "Build folder exist! Deleting..."
	rm -r -f build
fi
mkdir build
cd build
cmake ..
cmake --build . --target SDL Render -j 8 --config Debug
