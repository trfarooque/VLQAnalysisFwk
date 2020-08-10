#!/bin/bash

## ZJETS ##
MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_tag-21.2.87-htztx-3-syst_Int_Note/50_GeV_meffred_new/original' --OUTPUT='kinReweightings_OnlyZjets_PowPy8.root' --SIGNAL='Zjets,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets,Dibosons,ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,ttH,' --DEBUG=false --DOSMOOTHING=true --SYSTEMATICS=systematics_Vjets_pmg_list.txt

