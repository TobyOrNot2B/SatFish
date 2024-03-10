#!/bin/bash

# Input file name
input_file="input.txt"

# Output file name
output_file="output.txt"
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "No file provided."
  exit 1
fi

file=$1
if [ ! -f "$file" ]; then
  echo "File not found."
  exit 1
fi

# Get the line number of the first occurrence of '%'
line_number=$(grep -n '%' "$file" | head -n 1 | cut -d ':' -f 1)

if [ -n "$line_number" ]; then
  # Remove all lines after and including the line with '%'
  sed -i "${line_number},$ d" "$file"
else
  echo "No lines found with '%'"
fi

#find /some/directory -maxdepth 1 -type f -exec cmd option {} \; > results.out