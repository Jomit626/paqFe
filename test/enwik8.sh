#!/bin/bash

#set -e

PROJ_FOLDER="$(realpath $(dirname "$0")/..)"
PAQFE="$PROJ_FOLDER/build/paqfe"

DATA="$PROJ_FOLDER/test/big_data/enwik8"
#DATA=""$PROJ_FOLDER/test/data/bib""
OUT_DATA="$PROJ_FOLDER/tmp/enwik8.paqfe"
VALID_DATA="$PROJ_FOLDER/tmp/enwik8.bak"

echo - Start Compression Test on $DATA.
rm -rf $OUT_DATA* $VALID_DATA
#./paqfe -cv -i $PROJ_FOLDER/test/big_data/enwik8 -o $PROJ_FOLDER/tmp/enwik8.paqfe
compress_time=$({ /usr/bin/time -f %e -- $PAQFE -c -i $DATA -o $OUT_DATA; } 2>&1)
origin_size=$(stat -c "%s" $DATA)
compressed_size=$(stat -c "%s"  $OUT_DATA* | awk '{sum += $1} END{print sum}')
rate=$(echo "scale=6; $compressed_size / $origin_size * 100" | bc)
speed=$(echo "scale=2; $origin_size / $compress_time / 1024" | bc)

echo - Compressed in ${compress_time}s, ${speed} KB/s
echo - Compression ratio is $rate %.
echo - Start Decompression Test

decompress_time=$({ /usr/bin/time -f %e -- $PAQFE -x -o $VALID_DATA -i $OUT_DATA; } 2>&1)
speed=$(echo "scale=2; $compressed_size / $decompress_time / 1024" | bc)

cmp $DATA $VALID_DATA

echo - Decompression Test PASSED!
echo - Decompressed in ${decompress_time}s, ${speed} KB/s