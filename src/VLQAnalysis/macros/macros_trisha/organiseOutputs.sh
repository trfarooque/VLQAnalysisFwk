#!/bin/bash

INDIR=$1
CURDIR=${PWD}

cd ${INDIR}

mkdir ofiles
mkdir efiles
mkdir logfiles
mkdir HistFiles
mkdir TreeFiles

mv *.o* ofiles/.
mv *.e* efiles/.
mv *.log* logfiles/.
mv out*TREE.root TreeFiles/.
mv out*.root HistFiles/.

cd ${CURDIR}
