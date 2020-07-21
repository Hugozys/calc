#!/bin/bash
file=$(basename "$1")
echo $file
dot $1 -Tpng -o $file.png
eog $file.png 
