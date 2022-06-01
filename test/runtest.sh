#!/bin/bash

set -e

while getopts "w" arg
do
  case $arg in 
    w) OVER_WRITE=1;;
  esac
done

PROJ_FOLDER="$(realpath $(dirname "$0")/..)"
PQAFE="$PROJ_FOLDER/build/paqfe"

DATA_FOLDER="$PROJ_FOLDER/test/data/Silesia"
TMP_FOLDER="$PROJ_FOLDER/tmp"
LAST_RESULT_FILE="$TMP_FOLDER/result.tmp"

mkdir -p "$TMP_FOLDER"
touch $LAST_RESULT_FILE
LAST_RESULT=($(cat $LAST_RESULT_FILE))
result=""

echo -e file\\tresult\\torigin\\tcompressed\\trate
i=0
for file in $(ls $DATA_FOLDER)
do  
  # rm old files
  rm -f $TMP_FOLDER/$file.paqfe*

  # compress and decompress
  $PQAFE -c -i $DATA_FOLDER/$file -o $TMP_FOLDER/$file.paqfe > $TMP_FOLDER/$file.log
  # $PQAFE -x -i $TMP_FOLDER/$file.paqfe -o $TMP_FOLDER/$file > $TMP_FOLDER/$file.c.log

  # test result
  diff $DATA_FOLDER/$file $DATA_FOLDER/$file  >/dev/null
  diff_res=$?

  # cal ratio
  origin_size=$(stat -c "%s" $DATA_FOLDER/$file)
  compressed_size=$(stat -c "%s" $TMP_FOLDER/$file.paqfe* | awk '{sum += $1} END{print sum}')
  rate=$(echo "scale=6; $compressed_size / $origin_size * 100" | bc)

  delta=$(echo "scale=6; ${LAST_RESULT[$i]} - $rate" | bc)

  echo -e $file\\t$diff_res\\t$origin_size\\t$compressed_size\\t$rate\%\\t$delta\%
  
  result="$result $rate"
  i=($i+1)
done

if [ $OVER_WRITE ]
then
  echo "$result " > $LAST_RESULT_FILE
fi