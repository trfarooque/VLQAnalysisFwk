#!/bin/bash

VLQAna --outputFile=test_nominal_weight_systematics_v0.root \
--inputFile=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/nominal/user.farooque.410470.mc16e.ttbar.DAOD_TOPQ1.e6337_s3126_r10724_p3832.21.2.87-htztx-3-syst_output_tree.root/user.farooque.20501810._000001.output_tree.root \
--textFileList=false --sampleName=ttbarbb --sampleID=410470.mc16e \
--inputTree=nominal --isData=false --sampleDat=samples_info.tag-21.2.87-htztx-3-syst.mc16e.dat \
--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=false --splitVLQDecays=false \
--otherVariables=true --doBlind=false \
--useObjectSyst=false --useWeightSyst=false --onlyDumpSystHistograms=true \
--doOneLeptonAna=true --doZeroLeptonAna=false --doTwoLeptonAna=true --doSplitEMu=false \
--doLowBRegions=true --doLowJRegions=false \
--doFitRegions=true --doValidnRegions=true --DOSINGLEVLQREGIONS=true --doSingleVLQPreselection=true --doExtendedPreselection=false \
--doExclusiveJetRegions=true --DOPRESELSYS=true --DOPRESELECTION=true \
--computeWeightSys=false --DOEXPSYS=false --DOTHEORYSYS=false \
--ISAFII=false \
--MAXMETCUTTWOLEP=100. \
--useLeptonsSF=true --useLeptonTrigger=true --useMETTrigger=true --USEMETTRIGGERONELEP=true \
--applyMetMtwCuts=true --invertMetMtwCuts=false --APPLYMETSIGOBJCUT=false \
--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
--btagCollection=TRACK --trkJetPtCut=20. \
--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
--filterType=APPLYFILTER --useSlices=true --scaleTtbarHtSlices=false \
--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 --RCCollection=VR_rho550 \
--minMeffCut=600 \
--REWEIGHTKINEMATICS=true --KINRWLIST=JETSN,MEFFRED --DOKINRWSMOOTHING=true --DOKINRWSYST=true
