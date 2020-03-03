#!/bin/bash

VLQAna --outputFile=test.root \
--inputFile=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-0-fJvt/MV2//././user.orlando.364110.mc16e.Zjets.DAOD_TOPQ1.e5271_s3126_r10724_p3830.21.2.67-htztx-0-fJvt-MV2_output_tree.root/user.orlando.18837711._000001.output_tree.root \
--textFileList=false --sampleName=Zjets --computeWeightSys=false --sampleID=364100.mc16e \
--inputTree=nominal --isData=false --SAMPLEDAT=samples_info_tag-21.2.67-0-fJvt-MV2.mc16e.dat \
--DUMPHISTOS=true --DUMPOVERLAPTREE=false --DUMPTREE=false --DOTRUTHANALYSIS=false --SPLITVLQDECAYS=false \
--OTHERVARIABLES=true --DOBLIND=false --ONLYDUMPSYSTHISTOGRAMS=false \
--DOONELEPTONANA=false --DOZEROLEPTONANA=false --DOTWOLEPTONANA=true --DOSPLITEMU=true --DOSPLITFWDJET=false \
--DOLOWBREGIONS=true --DOLOWJREGIONS=false --DOFITREGIONS=false --DOVALIDNREGIONS=false \
--DOSINGLEVLQPRESELECTION=true --DOEXTENDEDPRESELECTION=false \
--DONEWMERGEBOOST=true --DONEWSUPERMERGEBOOST=false \
--USELEPTONSSF=true --USELEPTONTRIGGER=true --USEMETTRIGGER=true \
--APPLYMETMTWCUTS=true --INVERTMETMTWCUTS=false \
--APPLYDELTAPHICUT=true --MINDELTAPHICUT=0.4 --MAXDELTAPHICUT=-1. \
--DOTRF=false --RECOMPUTETRF=false --RECOMPUTEBTAGSF=true \
--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
--BTAGOP=FixedCutBEff_77 --BTAGCOLLECTION=TRACK --TRKJETPTCUT=20.\
--APPLYTTBARNNLOCORRECTION=false --APPLYVJETSSHERPA22RW=false --APPLYTTBBCORRECTION=false \
--FILTERTYPE=APPLYFILTER --SCALETTBARHTSLICES=false \
--JETPTCUT=25 --FWDJETPTCUT=20 --RCJETPTCUT=200 --RCNSUBJETSCUT=0 --RCCollection=VR_rho550 \
--REWEIGHTKINEMATICS=false --REWEIGHTNOMINALKINEMATICS=false --ONLYREWEIGHTTTBARKIN=true  --NEVENTS=50000
