#!/bin/bash

if [ -e build ]
then
	echo "Build folder exist! Deleting..."
	rm -r -f build
fi
mkdir build
cd build

if [ -n "$1" ]
then
	if [ -n "$2" ]
	then
		echo "CMAKE_BUILD_TYPE will be set to" $1
		echo "Will be used OFF-SCREEN rendering"
		cmake .. -DCMAKE_BUILD_TYPE="$1" -D FORCE_BUFFER_WINDOW=1
	else
		echo "CMAKE_BUILD_TYPE will be set to" $1
		cmake .. -DCMAKE_BUILD_TYPE="$1"
	fi
else
	echo "CMAKE_BUILD_TYPE will be set to Debug"
	cmake .. -DCMAKE_BUILD_TYPE=Debug
fi

cmake --build . -j 8
