#!/bin/bash

MACRODIR=/nfs/at3/scratch/cbuxovaz/VLQAnalysisFramework_br_pair_prod_ana/VLQAnalysisFramework/src/VLQAnalysis/macros/macros_stats
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_NEW_BOOST_TAGGING_BKG_RW_NEW_REGIONS/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_OPTIMIZED_BINNING_NO_QCD_V3_LIMITFITPREPLOTSPOSTPLOTS/
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_NEW_BOOST_TAGGING_BKG_RW/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_139IFB_NEWBOOSTEDOBJTAG_BKGRW_LIMITFITPREPLOTSPOSTPLOTS/
dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_BKG_RW_OLD_REGIONS/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_139IFB_OLDBOOSTEDOBJTAG_BKGRW_LIMITFITPREPLOTSPOSTPLOTS/
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING_V2/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV


#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_OLD_BOOST_TAGGING/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDANA_OLDBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_NOBKGRW/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDANA_NEWBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_5000GeV
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_NOBKGRW/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDANA_NEWBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_V5
#dirBase=/nfs/at3/scratch2/cbuxovaz/VLQAnalysisPairProdRun2_NOBKGRW/PairProd_tag-21.2.121-htztx-3/ASIMOV_BONLY_36IFB_OLDANA_NEWBOOSTEDOBJTAG_LIMITFITPREPLOTSPOSTPLOTS_V4
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=limitfitpreplotspostplots
#fitactions=preplotspostplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results_Fit_Limit/"

python ${MACRODIR}/LaunchTRExFitterOnBatch_newTRexF.py \
tarball=${tarpath} \
action=${fitactions} \
inputDir=${fitinputDir} outputDir=${fitoutputDir} \
