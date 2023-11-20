#!/bin/bash


MakeSByBPlot --SIGSAMPLEFILE=signal_list_0L_1200GeV.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=SIGNALS_1200 --VARIABLE=meff --DOOLDREGIONS=FALSE --DO1LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
--INDIR=/data/at3/scratch2/farooque/CarlosFitInputsTest/MVAHISTS/MergedFiles/nominal/

MakeSByBPlot --SIGSAMPLEFILE=signal_list_0L_1400GeV.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=SIGNALS_1400 --VARIABLE=meff --DOOLDREGIONS=FALSE --DO1LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
--INDIR=/data/at3/scratch2/farooque/CarlosFitInputsTest/MVAHISTS/MergedFiles/nominal/

MakeSByBPlot --SIGSAMPLEFILE=signal_list_0L_1600GeV.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=SIGNALS_1600 --VARIABLE=meff --DOOLDREGIONS=FALSE --DO1LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
--INDIR=/data/at3/scratch2/farooque/CarlosFitInputsTest/MVAHISTS/MergedFiles/nominal/

MakeSByBPlot --SIGSAMPLEFILE=signal_list_0L_2000GeV.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=SIGNALS_2000 --VARIABLE=meff --DOOLDREGIONS=FALSE --DO1LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
--INDIR=/data/at3/scratch2/farooque/CarlosFitInputsTest/MVAHISTS/MergedFiles/nominal/

#MakeSByBPlot --SIGSAMPLEFILE=signal_list_HtHt_0L.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=HtHt --VARIABLE=meff --DOOLDREGIONS=FALSE --DO0LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
#--INDIR=/data/at3/scratch2/tfarooque/VLQAnalysisRun2/VLQAnalysisOutputs_0L_fitRegions_statOnly/MergedFiles/nominal

#MakeSByBPlot --SIGSAMPLEFILE=signal_list_TDoublet_0L.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=TDoublet --VARIABLE=meff --DOOLDREGIONS=FALSE --DO0LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
#--INDIR=/data/at3/scratch2/tfarooque/VLQAnalysisRun2/VLQAnalysisOutputs_0L_fitRegions_statOnly/MergedFiles/nominal

#MakeSByBPlot --SIGSAMPLEFILE=signal_list_TSinglet_0L.txt --BKGSAMPLEFILE=bkg_list_0L.txt --OUTNAME=TSinglet --VARIABLE=meff --DOOLDREGIONS=FALSE --DO0LEPTON=TRUE --VARBIN=1000 --DOLATEXTABLE=TRUE \
#--INDIR=/data/at3/scratch2/tfarooque/VLQAnalysisRun2/VLQAnalysisOutputs_0L_fitRegions_statOnly/MergedFiles/nominal
