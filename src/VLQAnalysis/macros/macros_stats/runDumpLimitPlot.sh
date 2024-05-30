#!/bin/bash

python DumpLimitPlot.py -i /msu/data/t3work10/cbuxo/TRexFitter_Output/ASIMOV_BONLY/Results_Fit_Limit_V1/ -o limit_plots -s TTHTHT -l 139 -t --forceranges

python DumpLimitPlot.py -i /msu/data/t3work10/cbuxo/TRexFitter_Output/ASIMOV_BONLY/Results_Fit_Limit_V1/ -o limit_plots -s TTD -l 139 -t --forceranges

python DumpLimitPlot.py -i /msu/data/t3work10/cbuxo/TRexFitter_Output/ASIMOV_BONLY/Results_Fit_Limit_V1/ -o limit_plots -s TTS -l 139 -t --forceranges
