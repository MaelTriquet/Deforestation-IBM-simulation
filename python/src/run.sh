#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <integer>"
    exit 1
fi

# Check if the argument is an integer
if ! [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "Error: Argument must be an integer."
    exit 1
fi
RES_PATH="/home/hugo/cours/ing3/pfe/res/bayes"
# Create a directory named "settings_$1" where $1 is the provided argument
SETTINGS_DIR="$RES_PATH/settings_$1"
mkdir -p "$SETTINGS_DIR"

cd "/home/hugo/cours/ing3/pfe/c++/build"
# Compile the C++ code using make
make

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"

    # Repeat 3 times
    for i in {1..3}; do
        # Create a directory named "run_i" where i is the iteration
        RUN_DIR="$SETTINGS_DIR/run_$i"
        mkdir -p "$RUN_DIR"

        # Run the executable
        echo "Running the program (Attempt $i):"
        ./MySFMLProject > "$RUN_DIR/results.txt"
        echo "Output saved to $RUN_DIR/results.txt"
        echo "Score : "
        cat "$RUN_DIR/results.txt"

        cp "/home/hugo/cours/ing3/pfe/res/plot_info.csv" "$RUN_DIR"
        echo "saved csv file to $RUN_DIR/"
        # Copy the file "{path}/const.cpp" to the "settings_$1" directory
        cp "/home/hugo/cours/ing3/pfe/c++/src/const.hpp" "$SETTINGS_DIR/"
        echo "Copied const.cpp to $SETTINGS_DIR/"
    done
else
    echo "Compilation failed!"
fi

cd -