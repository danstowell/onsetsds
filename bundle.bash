#!/bin/bash

# Script to make a download bundle of the OnsetsDS code
# Dan Stowell, June 2008
version=`date "+%Y-%m-%d"`

# build the documentation
cd doc
doxygen
cd ..

rm -rf OnsetsDS

svn export . OnsetsDS
cp -R doc/html OnsetsDS/doc/html

zip -r "OnsetsDS-$version" OnsetsDS
tar -czvf "OnsetsDS-$version.tgz" OnsetsDS

