#!/bin/bash

set -e

PROJ_FOLDER="$(dirname "$0")/.."

DB_NAME=$1
PROG=$2

DATA_FOLDER="$PROJ_FOLDER/verify/data"
DB_FOLDER="$PROJ_FOLDER/verify/db"
DB_FILE=$DB_FOLDER/$DB_NAME

mkdir -p $DB_FOLDER
rm -f $DB_FILE

for file in $(ls $DATA_FOLDER)
do  
  INPUT_FILE="$(realpath $DATA_FOLDER/$file)"
  OUTPUT_FILE="$(realpath $DB_FOLDER/$DB_NAME-$file)"

  echo $file,$INPUT_FILE,$OUTPUT_FILE >> $DB_FILE

  $PROG -i $INPUT_FILE -o $OUTPUT_FILE
done
