#!/bin/bash

MakeSByBPlot --SAMPLEFILE=signal_list_TTDoublet.txt --OUTNAME=Doublet --VARIABLE=meff --DOOLDREGIONS=FALSE --VARBIN=1000

MakeSByBPlot --SAMPLEFILE=signal_list_TTSinglet.txt --OUTNAME=Singlet --VARIABLE=meff --DOOLDREGIONS=FALSE --VARBIN=1000

MakeSByBPlot --SAMPLEFILE=signal_list_HtHt.txt --OUTNAME=HtHt --VARIABLE=meff --DOOLDREGIONS=FALSE --VARBIN=1000

MakeSByBPlot --SAMPLEFILE=signal_list_HtWb.txt --OUTNAME=HtWb --VARIABLE=meff --DOOLDREGIONS=FALSE --VARBIN=1000

MakeSByBPlot --SAMPLEFILE=signal_list_HtZt.txt --OUTNAME=HtZt --VARIABLE=meff --DOOLDREGIONS=FALSE --VARBIN=1000
