#!/bin/bash

fList="${PWD}/SIGPLOT_file_list.txt"

styleLib="${PWD}/IFP_shape_style_lib_lines.txt"

newConfig="TRUE"
newFileList="FALSE"
showYields="TRUE"
outFormat="PNG,EPS"
lumi=13207.69
#lumi=3209.
ylabel="Arbitrary Units"
ttl_xmin=0.18
ttl_ymin=0.76
ttl_xmax=0.38
ttl_ymax=0.87 
#blinding="THRESH"
#blindThresh=0.02
y_title_off=1.5 
maxxleg=0.85



declare -a OutFList=("Hplus" "bbH" "ttH")
declare -a ConfList=("${PWD}/Shape_Hplus_sample_config.txt" "${PWD}/Shape_bbH_sample_config.txt" "${PWD}/Shape_ttH_sample_config.txt")

#variableConfig="${PWD}/SHAPE_variable_config.txt"
variableConfig="${PWD}/TEST_variable_config.txt"

arraylength=${#OutFList[@]}

# use for loop to read all values and indexes
for (( i=1; i<${arraylength}+1; i++ ));
do
    echo $i " / " ${arraylength} " : Plots in " ${OutFList[$i-1]}
    
    plot --SAMPLES=${ConfList[$i-1]} --VARIABLES=${variableConfig} --FILELIST=${fList} --STYLELIB=${styleLib} \
	--NEWCONFIG=${newConfig} --NEWFILELIST=${newFileList} --OUTPUTFOLDER=${OutFList[$i-1]} \
	--SHOWYIELDS=${showYields} --OUTFORMAT=${outFormat} --GLOBALSCALE=${lumi} --YLABEL="Events (norm. to unity)" \
	--TITLEXMIN=${ttl_xmin} --TITLEYMIN=${ttl_ymin} --TITLEXMAX=${ttl_xmax} --TITLEYMAX=${ttl_ymax} \
	--RESMIN="0.2" --RESMAX="1.8" --RESDRAWOPT="hist" --RESLABEL="Ratio to Nominal" --YTITLEOFFSET=${y_title_off} --LEGENDXMAX=${maxxleg}     #--WRITEHISTOS=TRUE #--RESLABEL="Merged/Inclusive" 
  
done


