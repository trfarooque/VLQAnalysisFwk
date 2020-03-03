#!/bin/bash

mkdir ../VLQFramework
mkdir ../VLQFramework/VLQAnalysisFramework
mv ../VLQAnalysis ../VLQFramework/VLQAnalysisFramework

setupATLAS

mv CMakeGlobal/CMakeLists.txt ../
rm -rf GlobalCMake

cd ../

git clone ssh://git@gitlab.cern.ch:7999/htx/IFAETopFramework.git
cd IFAETopFramework && git checkout IFAETopFramework_CMake && cd ..

git clone ssh://git@gitlab.cern.ch:7999/htx/BtaggingTRFandRW.git
cd BtaggingTRFandRW && git checkout BtaggingTRFandRW_CMake && cd ..

git clone ssh://git@gitlab.cern.ch:7999/htx/IFAEReweightingTools.git
cd IFAEReweightingTools && git checkout IFAEReweightingTools_CMake && cd ..

git clone ssh://git@gitlab.cern.ch:7999/htx/FakeLeptonEstimation.git
cd FakeLeptonEstimation && git checkout FakeLeptonEstimation_CMake && cd ..

git clone ssh://git@gitlab.cern.ch:7999/htx/IFAEPlotter.git
cd IFAEPlotter && git checkout IFAEPlotter_CMake && cd ..

cd ../ && mkdir build && mkdir run
cd build


asetup 21.2.75,AnalysisBase

cmake ../VLQAnalysisFramework

cd ../VLQAnalysisFramework/VLQAnalysis


mv compileScript.sh ../
mv setupScript.sh ../

cd ../

source compileScript.sh
