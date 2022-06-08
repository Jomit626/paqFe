#!/bin/bash
set -e

PROJ_FOLDER="$(dirname "$0")/.."
DATA_FOLDER="$PROJ_FOLDER/test/data/Silesia"

mkdir -p $DATA_FOLDER
# mkdir -p $VERIFY_DATA_FOLDER

cd $DATA_FOLDER

wget https://sun.aei.polsl.pl//~sdeor/corpus/silesia.zip
unzip silesia.zip -d ./
rm silesia.zip

cd -