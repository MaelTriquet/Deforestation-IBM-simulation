#!/bin/bash

# Define the executable name
EXECUTABLE="main"

# Compile the C++ code using make
make

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"

    # Run the executable three times
    for i in {1..3}; do
        echo "Running the program (Attempt $i):"
        ./$EXECUTABLE
        echo ""
    done
else
    echo "Compilation failed!"
fi
