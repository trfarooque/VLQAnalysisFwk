#!/bin/bash

MVATrainer \
--inputTreeDir="/data/at3/scratch2/farooque/PVLQ_MVATrainingTrees/" \
--outFileName="testTraining.root" \
--prunedVarFileName="" \
--signalMasses="1200,1400,1600,1800" \
--backgrounds="ttbarlight,ttbarcc,ttbarbb" \
--channel="ONELEP" \
--mlpNodes="10,5" \
--mlpNCycles="200" \
--mlpNeuronType="sigmoid" \
--nFolds="5" \
--doCrossEval="TRUE" \
--useAllVars="FALSE" \
--usePrunedVars="FALSE" \
--varList="meff,met,jets_n,bjets_n"

#--mlpNodes="30,10,10,5" \
#--mlpNCycles="700" \
#--mlpNeuronType="sigmoid" \
#--nFolds="5" \
