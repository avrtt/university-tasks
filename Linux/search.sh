#!/bin/bash

if grep -q "$1" database.txt; then
  grep "$1" database.txt 
else
  echo "No matches found."
fi
