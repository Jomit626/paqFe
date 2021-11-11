#!/bin/bash
mkidr -p data
cd data

wget http://corpus.canterbury.ac.nz/resources/calgary.tar.gz
tar -xf calgary.tar.gz
rm calgary.tar.gz

cd -