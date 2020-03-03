#!/bin/bash

setupATLAS

cd ../build

asetup 21.2.75,AnalysisBase

make

source x86_64-centos7-gcc62-opt/setup.sh

export BUILDDIR=$(pwd)
export PYTHONPATH=$(pwd)/x86_64-centos7-gcc62-opt/python/VLQAnalysis/:$(pwd)/x86_64-centos7-gcc62-opt/python/IFAETopFramework/:$PYTHONPATH

cd ../VLQAnalysisFramework
