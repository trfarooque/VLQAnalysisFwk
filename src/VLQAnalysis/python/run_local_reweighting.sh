#!/bin/bash

VLQAna --outputFile=test_nominal_weight_systematics.root \
--inputFile=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-0-fJvt/MV2//././user.farooque.407342.mc16a.ttbar.DAOD_TOPQ1.e6414_s3126_r9364_p3832.21.2.67-htztx-0-fJvt-MV2.nonempty.v2_output_tree.root/user.farooque.19138825._000001.output_tree.root \
--textFileList=false --sampleName=ttbarbb --computeWeightSys=false --sampleID=407342.mc16e \
--inputTree=nominal --isData=false --SAMPLEDAT=samples_info_tag-21.2.67-0-fJvt-MV2.mc16e.dat \
--DUMPHISTOS=true --DUMPOVERLAPTREE=false --DUMPTREE=false --DOTRUTHANALYSIS=false --SPLITVLQDECAYS=false \
--OTHERVARIABLES=false --DOBLIND=false --ONLYDUMPSYSTHISTOGRAMS=true \
--DOONELEPTONANA=true --DOZEROLEPTONANA=false --DOTWOLEPTONANA=true --DOSPLITEMU=false \
--DOLOWBREGIONS=true --DOLOWJREGIONS=false --DOFITREGIONS=true --DOVALIDNREGIONS=true \
--DOEXTENDEDPRESELECTION=false \
--DOSINGLEVLQREGIONS=true --doExclusiveJetRegions=true \
--USELEPTONSSF=true --USELEPTONTRIGGER=true --USEMETTRIGGER=true \
--APPLYMETMTWCUTS=true --INVERTMETMTWCUTS=false \
--APPLYDELTAPHICUT=true --MINDELTAPHICUT=0.4 --MAXDELTAPHICUT=-1. \
--APPLYMETSIGCUT=false --APPLYMETSIGOBJCUT=false \
--ISAFII=false \
--DOTRF=false --RECOMPUTETRF=false --RECOMPUTEBTAGSF=true \
--TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
--BTAGOP=FixedCutBEff_77 --BTAGCOLLECTION=TRACK --TRKJETPTCUT=20.\
--APPLYTTBARNNLOCORRECTION=false --APPLYVJETSSHERPA22RW=false --APPLYTTBBCORRECTION=false \
--FILTERTYPE=APPLYFILTER --SCALETTBARHTSLICES=false \
--JETPTCUT=25 --FWDJETPTCUT=20 --RCJETPTCUT=200 --RCNSUBJETSCUT=0 --RCCollection=VR_rho550 \
--REWEIGHTKINEMATICS=true --NEVENTS=50000 --KINRWLIST=JETSN,MEFF
