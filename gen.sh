#!/bin/bash
file=$(basename "$1")
base=$(basename -s .dot "$1")
dot $1 -Tpng -o $base.png
