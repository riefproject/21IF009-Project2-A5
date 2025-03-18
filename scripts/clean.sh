#!/bin/bash

echo "Cleaning build directories..."

# Remove the bin directory if it exists
if [ -d "bin" ]; then
    rm -rf bin
    echo "Removed bin directory."
fi

# Remove the build/output directory if it exists
if [ -d "build/output" ]; then
    rm -rf build/output
    echo "Removed build/output directory."
fi

echo "Clean complete!"
