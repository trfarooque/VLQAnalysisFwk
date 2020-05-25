#!/bin/bash


#/nfs/at3/scratch2/farooque/user.farooque.364197.mc16e.Wjets.DAOD_TOPQ1.e5340_s3126_r10724_p3830.21.2.87-htztx-3-syst-VJETSSYST_output_tree.root/user.farooque.20745748._000002.output_tree.root


inDir=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst-VJETSSYST

#for each dataset
for dataset in `ls $inDir`
do
    if [[ $dataset == *"Zjets"* ]]; then
	dsID=${dataset#user.farooque.}
	dsID=${dsID%%.mc16*output_tree.root}
	#echo "dsID : ${dsID}"
	#echo "dataset : ${dataset}"
	for file in `ls ${inDir}/${dataset}/`
	do
	    filepath=${inDir}/${dataset}/${file}
	    echo "python AddZJetsWeights.py ${filepath} --treename nominal --mcID ${dsID} --nTruthJets truthjets_n"
	    python AddZJetsWeights.py ${filepath} --treename nominal --mcID ${dsID} --nTruthJets truthjets_n
	done
    fi
done
