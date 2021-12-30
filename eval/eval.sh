#!/bin/bash

set -e

PROJ_FOLDER="$(dirname "$0")/.."
PROG="$PROJ_FOLDER/build/orders-eval"

DATA_FOLDER="$PROJ_FOLDER/test/data"
RESULT_FOLDER="$PROJ_FOLDER/eval/result"

mkdir -p $RESULT_FOLDER
i=0
for file in $(ls $DATA_FOLDER)
do  
  # rm old files
  rm -f $RESULT_FOLDER/$file.eval*

  $PROG -c -i $DATA_FOLDER/$file -o $RESULT_FOLDER/$file.eval
done
