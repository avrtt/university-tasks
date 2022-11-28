#!/bin/bash

if [ ! -e $1 ] 
then
    echo "Error: Directory does not exist."
    exit 1
fi

if [ ! $# -eq 1 ] 
then 
    echo "Error: Directory name not specified or incorrect."
    exit 1
fi


if [ -f $1 ] 
then
    echo "Error: Not a directory."
    exit 1
fi

cd $1

until [ "`pwd`" = "/" ] 
do
    CURRENT=`pwd`
    echo -e "\n   $(basename "$CURRENT")"
    ls -1F | grep '/$'| sed 's/\/$//' | 
        while read Name
        do
            echo "        $Name"
        done
    cd ..
done

cd ..
echo -e "\n   /"
ls -1FS | grep '/$'| sed 's/\/$//' | 
    while read Name
    do
        echo "        $Name"
    done
