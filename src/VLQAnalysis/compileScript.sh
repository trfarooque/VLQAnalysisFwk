#!/bin/bash

setupATLAS

cd ../../build

asetup 21.2.87,AnalysisBase

make

source */setup.sh

export BUILDDIR=$(pwd)
export PYTHONPATH=$(pwd)/x86_64-centos7-gcc62-opt/python/VLQAnalysis/:$(pwd)/x86_64-centos7-gcc62-opt/python/IFAETopFramework/:$PYTHONPATH

cd ../src
