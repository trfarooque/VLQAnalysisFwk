#!bin/bash

for decay in "HtHt" "ZtZt"
do
    for mass in "1400" "1600" "1800" "2000"
    do 
	python MVA_Evaluation_Plotter.py -r -s -e -O output_1lep6jin3bin2Min3Jin/${mass} -H c1lep6jin3bin2Min3Jin_*MVAScore -M ${mass} -D ${decay} -L
    done
done

for mass in "1400" "1600" "1800" "2000"
do
    python MVA_Evaluation_Plotter.py -r -s -e -O output_1lep6jin3bin2Min3Jin/${mass} -H c1lep6jin3bin2Min3Jin_*MVAScore -M ${mass} -D "HtZt" -P "vlq3_" -S "4.5" -L
done
