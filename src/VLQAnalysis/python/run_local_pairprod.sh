#!/bin/bash

VLQAna --outputFile=test_pairprod_signal.root \
--inputFile=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples//././user.cbuxovaz.302472.mc16a.pairvlq.DAOD_TOPQ1.e7645_s3126_r9364_p4031.21.2.121-htztx-3-syst_output_tree.root/user.cbuxovaz.22885490._000001.output_tree.root \
--textFileList=false --sampleName=VLQ_TT_800 --sampleID=302472.mc16a \
--inputTree=nominal --isData=false --sampleDat=samples_info.tag-21.2.121-htztx-3-syst-PAIRVLQ.mc16a.dat \
--dumpHistos=true --dumpOverlapTree=false --dumpTree=false --doTruthAnalysis=true --verboseOutput=true --splitVLQDecays=true \
--otherVariables=true --doBlind=false \
--onlyDumpSystHistograms=true \
--doOneLeptonAna=true --doZeroLeptonAna=false --doTwoLeptonAna=false --doSplitEMu=false --splitVLQDecays=true \
--doLowBRegions=true --doLowJRegions=false \
--doFitRegions=true --doValidnRegions=true --DOSINGLEVLQREGIONS=false --doPairVLQRegions=true --doExtendedPreselection=false \
--doExclusiveJetRegions=false --DOPRESELSYS=true --DOPRESELECTION=true \
--computeWeightSys=false --DOEXPSYS=false --DOTHEORYSYS=false --DOPDFSYS=false \
--ISAFII=false \
--MAXMETCUTTWOLEP=100. \
--doRecoVLQ=pair \
--useLeptonsSF=true --useLeptonTrigger=true --useMETTrigger=true --USEMETTRIGGERONELEP=true \
--applyMetMtwCuts=true --invertMetMtwCuts=false --APPLYMETSIGOBJCUT=false \
--applydeltaphicut=true --minDeltaPhiCut=0.4 --maxDeltaPhiCut=-1. \
--doTRF=false --recomputeTRF=false --recomputeBTagSF=false --btagOP=FixedCutBEff_77 \
--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
--btagCollection=TRACK --trkJetPtCut=20. \
--applyTtbarNNLOCorrection=false --applyVJetsSherpa22RW=false --applyTtbbCorrection=false \
--filterType=APPLYFILTER --scaleTtbarHtSlices=false \
--jetPtCut=25 --fwdJetPtCut=20 --RCJetPtCut=200 --RCNsubjetsCut=0 --RCCollection=VR_rho550 \
--minMeffCut=600 \
--DERIVEREWEIGHTING=true \
--REWEIGHTKINEMATICS=false --DOKINRWSMOOTHING=false --DOKINRWSYST=false
