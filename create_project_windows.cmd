if exist build\ (
	echo "Build folder exist! Deleting..."
	RD /S /Q build
)
mkdir build
cd build
cmake ..
cmake --build . -j 8 --config Release