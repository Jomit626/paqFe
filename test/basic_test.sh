#!/bin/bash

echo Hello, World! > /tmp/Hello
cat /tmp/Hello

rm -rf /tmp/Hello.paq /tmp/Hello.bak
./paqfe -cv -i /tmp/Hello -o /tmp/Hello.paq
./paqfe -xv -o /tmp/Hello.bak -i /tmp/Hello.paq

cat /tmp/Hello.bak