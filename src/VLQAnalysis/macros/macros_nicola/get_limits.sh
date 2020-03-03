#!/bin/bash

DIRS="batch_Moriond_5j0J_13_12_2016_v0 
batch_Moriond_5j_13_12_2016_v0   
batch_Moriond_6j_13_12_2016_v0
batch_Top_13_12_2016_v0
batch_Moriond_5jplusHighNj_31_12_2016_v0"          

tttt_proc="tttt_400
tttt_500
tttt_600
tttt_700
tttt_800
tttt_900
tttt_1000"

ttbb_proc="ttbb_400
ttbb_500
ttbb_600
ttbb_700
ttbb_800
ttbb_900
ttbb_1000"

Hplus_proc="Hplus_200
Hplus_250
Hplus_300
Hplus_350
Hplus_400
Hplus_500
Hplus_600
Hplus_700
Hplus_800
Hplus_900
Hplus_1000
Hplus_1200
Hplus_1400
Hplus_1600
Hplus_1800
Hplus_2000"

for dir in $DIRS
do
    echo "Processing $dir"

    #for tttt in tttt_proc
    #do 
    
    grep Median ${dir}/${dir}_tttt*/limit.log | sed -r "s/${dir}\/${dir}_tttt_//" | sed -r "s/\/limit.log:Median://" >> loglim_${dir}_tttt.log

    grep Median ${dir}/${dir}_ttbb*/limit.log  | sed -r "s/${dir}\/${dir}_ttbb_//" | sed -r "s/\/limit.log:Median://" >> loglim_${dir}_ttbb.log

    grep Median ${dir}/${dir}_Hplus*/limit.log | sed -r "s/${dir}\/${dir}_Hplus_//" | sed -r "s/\/limit.log:Median://" >> loglim_${dir}_Hplus.log 

done