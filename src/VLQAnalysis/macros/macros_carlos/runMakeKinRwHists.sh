#!/bin/bash

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_PowPy8.root' --SIGNAL='ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,' 

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyZjets_PowPy8.root' --SIGNAL='Zjets,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets,Dibosons,ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,ttH,'


## TTBAR ALT GEN ##

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_PowH7.root' --SIGNAL='ttbarlightPowHer,ttbarccPowHer,ttbarbbPowHer,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_aMCPy.root' --SIGNAL='ttbarlightaMCPy,ttbarccaMCPy,ttbarbbaMCPy,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_AFII.root' --SIGNAL='ttbarlightAFII,ttbarccAFII,ttbarbbAFII,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'

## OLD FILES ##

#MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020/Zjets_reweighting_v2' --OUTPUT='kinReweightings_OnlyTtbar.root' --SIGNAL='ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets,Zjets,Dibosons,'

#MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020/original' --OUTPUT='kinReweightings_OnlyTtbar.root' --SIGNAL='Zjets,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets,Dibosons,ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,'
