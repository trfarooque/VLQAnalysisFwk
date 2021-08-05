#!/bin/bash

python DumpLimitPlot.py -i /nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_V2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV/Results_Fit_Limit/ -o pVLQ_old_boost_tagging -s TTZTZT -l 36.1 --forceranges

python DumpLimitPlot.py -i /nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_V2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV/Results_Fit_Limit/ -o pVLQ_old_boost_tagging -s TTHTHT -l 36.1 --forceranges

python DumpLimitPlot.py -i /nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_V2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV/Results_Fit_Limit/ -o pVLQ_old_boost_tagging -s TTD -l 36.1 --forceranges

python DumpLimitPlot.py -i /nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_V2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV/Results_Fit_Limit/ -o pVLQ_old_boost_tagging -s TTS -l 36.1 --forceranges
