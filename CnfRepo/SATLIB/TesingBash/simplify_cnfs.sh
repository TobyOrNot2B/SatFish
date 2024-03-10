#!/bin/bash

# Check if directory path and command are provided as arguments
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

# Get the directory path and command from the command line arguments
dir_path=$1
command="$PWD/SatELite_v1.0_linux"

# Check if the directory path is a valid directory
if [ ! -d "$dir_path" ]; then
    echo "Error: $dir_path is not a directory"
    exit 1
fi

# Loop through all files in the directory
for file in "$dir_path"/*; do
    # Check if file is a regular file
    if [ -f "$file" ]; then
        # Get the filename without the directory path
        filename=$(basename "$file")
        # make the output directory
        output_dir="$dir_path-simplified"
        mkdir -p "$output_dir"
        # get the output path
        output_path="$output_dir/$filename"
        # Execute the command on the file and capture the output
        result=$(eval "$command \"$file\" \"$output_path\" +ve+ +det | grep -oP 'CPU time:\s+\K[0-9.]+(?=\s*s)'")
        # Output the filename and result
        echo "$filename, $result"
    fi
done

# ./simplify_cnfs.sh "./test" > simpdata.csv