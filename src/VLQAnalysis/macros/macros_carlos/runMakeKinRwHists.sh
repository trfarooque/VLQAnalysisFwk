#!/bin/bash

## ZJETS ##
MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyZjets_PowPy8.root' --SIGNAL='Zjets,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets,Dibosons,ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,ttH,'

## TTBAR AND WT NOMINAL ##
MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_PowPy8.root' --SIGNAL='ttbarlight,ttbarcc,ttbarbb,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,' 

## TTBAR ALTGEN/AFII AND WT ##

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_PowH7.root' --SIGNAL='ttbarlightPowHer,ttbarccPowHer,ttbarbbPowHer,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_aMCPy.root' --SIGNAL='ttbarlightaMCPy,ttbarccaMCPy,ttbarbbaMCPy,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'

MakeKinRwHists --INDIR='/nfs/at3/scratch2/cbuxovaz/VLQAnalysisRun2/NewProd_2020_v2/original' --OUTPUT='kinReweightings_OnlyWtTtbar_AFII.root' --SIGNAL='ttbarlightAFII,ttbarccAFII,ttbarbbAFII,SingletopWtprod,' --BACKGROUND='Singletoptchan,Singletopschan,topEW,Wjets_RW,Zjets_RW,Dibosons,ttH,'
