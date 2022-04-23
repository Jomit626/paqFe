#!/bin/bash
set -e

PROJ_FOLDER="$(realpath $(dirname "$0")/..)"
DATA_FOLDER="$PROJ_FOLDER/test/data"
VERIFY_DATA_FOLDER="$PROJ_FOLDER/verify/data"

mkdir -p $VERIFY_DATA_FOLDER

cat $DATA_FOLDER/book1 | head -n 8 > $VERIFY_DATA_FOLDER/small
cat $DATA_FOLDER/book2 | head -n 64 > $VERIFY_DATA_FOLDER/medium
dd if=/dev/zero of=$VERIFY_DATA_FOLDER/zeros count=100 bs=1024 2>/dev/null
