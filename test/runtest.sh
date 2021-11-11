#!/bin/bash

PROJ_FOLDER="$(dirname "$0")/.."
PQAFE="$PROJ_FOLDER/build/paqFe"

DATA_FOLDER="$PROJ_FOLDER/test/data"
TMP_FOLDER="$PROJ_FOLDER/tmp"
echo -e file\\tresult\\torigin\\tcompressed\\trate
for file in $(ls $DATA_FOLDER)
do
  $PQAFE -c -i $DATA_FOLDER/$file -o $TMP_FOLDER/$file.paqfe
  $PQAFE -x -i $TMP_FOLDER/$file.paqfe -o $TMP_FOLDER/$file >/dev/null
  diff $DATA_FOLDER/$file $TMP_FOLDER/$file  >/dev/null
  diff_res=$?
  origin_size=$(stat -c "%s" $DATA_FOLDER/$file)
  compressed_size=$(stat -c "%s" $TMP_FOLDER/$file.paqfe)
  rate=$(echo "scale=6; $compressed_size / $origin_size * 100" | bc)
  echo -e $file\\t$diff_res\\t$origin_size\\t$compressed_size\\t$rate\%
done
