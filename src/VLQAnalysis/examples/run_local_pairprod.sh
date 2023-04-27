
#!/bin/bash
#--inputFile=/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples/user.cbuxovaz.302482.mc16a.pairvlq.DAOD_TOPQ1.e7645_s3126_r9364_p4031.21.2.121-htztx-3-syst_outp\ut_tree.root/user.cbuxovaz.22885496._000001.output_tree.root \

VLQAna --outputFile=newOHMngr_newVD.root \
--inputFile=/data/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_ntuples/21.1.121-htztx-3-syst_0_lepton/user.cbuxovaz.302482.mc16e.pairvlq0L.DAOD_TOPQ1.e7645_s3126_r10724_p4031.21.2.121-htztx-3-syst_output_tree.root/user.cbuxovaz.25475592._000001.output_tree.root \
--textFileList=false --sampleName=VLQ_TT_1400 --sampleID=302482.mc16e \
--inputTree=nominal --isData=false --sampleDat=samples_info/samples_info.tag-21.2.121-htztx-3-syst-PAIRVLQ.mc16e.dat \
--makeMVAInputHists=false --makeMVAInputTree=false \
--dumpHistos=true --dumpOverlapTree=false --dumpTree=false \
--doTruthAnalysis=false --verboseOutput=false --splitVLQDecays=false \
--otherVariables=true --doBlind=false \
--onlyDumpSystHistograms=false \
--doOneLeptonAna=false --doZeroLeptonAna=true --doTwoLeptonAna=false --doSplitEMu=false --splitVLQDecays=false \
--doLowBRegions=false --doLowJRegions=false \
--doFitRegions=false --doValidnRegions=false \
--DOSINGLEVLQREGIONS=false --doPairVLQRegions=true --doExtendedPreselection=false \
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
--minMeffCut=1000 \
--DERIVEREWEIGHTING=false \
--REWEIGHTKINEMATICS=false --DOKINRWSMOOTHING=false --DOKINRWSYST=false --doLooseSystRegions=false \
--maxLeptopDR=1.5 --leptopOpt=USE_LIGHT,PREF_BTAG \
--applyMVA=FALSE #--msglevel=DEBUG #--mvaWeightFile=TMVA/TMVAClassification_MLP.weights_0lep_meffin.xml
#--applyMVA=TRUE --mvaWeightFile=TMVA/weightsCV_1L/TMVAClassificationCV_MLP.weights.xml
 #--lepWOpt=MW_CONSTRAINT

#_fold3.weights
