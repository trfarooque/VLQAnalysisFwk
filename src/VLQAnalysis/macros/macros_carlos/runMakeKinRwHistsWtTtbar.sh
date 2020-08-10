#!/bin/bash

#_two_lep_met_cut

## TTBAR AND WT NOMINAL ##                                                                                                                                                                               
MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_tag-21.2.87-htztx-3-syst_Int_Note/50_GeV_meffred_new/original' --OUTPUT='kinReweightings_OnlyWtTtbar_PowPy8.root' --SIGNAL='ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW_smooth,Zjets_RW_smooth,Dibosons,ttH,' --DEBUG=false --DOSMOOTHING=true --SYSTEMATICS=systematics_list.txt
