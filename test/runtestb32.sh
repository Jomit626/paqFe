#!/bin/bash

set -e

BLOCKSIZE=32

PROJ_FOLDER="$(realpath $(dirname "$0")/..)"
PQAFE="$PROJ_FOLDER/build/paqfebw"

DATA_FOLDER="/home/jomit/tmp/testdata/blocktest"
TMP_FOLDER="/home/jomit/tmp/testdata/tmp$BLOCKSIZE"

mkdir -p "$TMP_FOLDER"
rm -rf "$TMP_FOLDER"
mkdir -p "$TMP_FOLDER"

result=""
echo $BLOCKSIZE KB
echo -e file\\tresult\\torigin\\tcompressed\\trate
for file in $(ls $DATA_FOLDER)
do  
  # rm old files
  rm -f $TMP_FOLDER/$file.paqfe*

  # compress and decompress
  $PQAFE -vc -b $BLOCKSIZE -i $DATA_FOLDER/$file -o $TMP_FOLDER/$file.paqfe > $TMP_FOLDER/$file.log

  # test result
  diff_res="x"

  # cal ratio
  origin_size=$(stat -c "%s" $DATA_FOLDER/$file)
  compressed_size=$(stat -c "%s" $TMP_FOLDER/$file.paqfe* | awk '{sum += $1} END{print sum}')
  rate=$(echo "scale=6; $compressed_size / $origin_size * 100" | bc)


  echo -e $file\\t$diff_res\\t$origin_size\\t$compressed_size\\t$rate\%
  
done
