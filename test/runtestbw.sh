#!/bin/bash
# Test block wise
# usage: runtestbw.sh [block size]

BLOCKSIZE=128

set -e
re='^[0-9]+$'
if [[ $1 =~ $re ]] ; then
  BLOCKSIZE=$1
fi
echo Block Size: $BLOCKSIZE bytes

PROJ_FOLDER="$(realpath $(dirname "$0")/..)"
PQAFE="$PROJ_FOLDER/build/paqfebw"

DATA_FOLDER="$PROJ_FOLDER/test/data"
TMP_FOLDER="$PROJ_FOLDER/tmp$BLOCKSIZE"

mkdir -p "$TMP_FOLDER"
rm -rf "$TMP_FOLDER"
mkdir -p "$TMP_FOLDER"

result=""
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
  compressed_size=$(ls -l $TMP_FOLDER | grep $file | awk '{sum += $5} END{print sum}')
  rate=$(echo "scale=6; $compressed_size / $origin_size * 100" | bc)


  echo -e $file\\t$diff_res\\t$origin_size\\t$compressed_size\\t$rate\%
  
done
