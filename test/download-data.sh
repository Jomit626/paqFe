#!/bin/bash
set -e

PROJ_FOLDER="$(dirname "$0")/.."
DATA_FOLDER="$PROJ_FOLDER/test/data"

mkdir -p $DATA_FOLDER
mkdir -p $VERIFY_DATA_FOLDER

cd $DATA_FOLDER

wget http://corpus.canterbury.ac.nz/resources/calgary.tar.gz
tar -xf calgary.tar.gz
rm calgary.tar.gz

cd -

