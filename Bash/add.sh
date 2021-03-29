#!/bin/bash

steps=(10 30 32 50)

for i in $1 $2 $3 $4 $5
do
  echo -n $i >> database.txt
  let "len_param=len_param+`expr length $i`"

  while [[ $len_param -lt ${steps[$k]} ]]
  do
    echo -n " " >> database.txt
    let "len_param=len_param+1"
  done

  let "k=k+1"
done

echo "" >> database.txt
