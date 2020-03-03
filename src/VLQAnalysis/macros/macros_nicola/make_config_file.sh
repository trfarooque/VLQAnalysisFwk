#!/bin/bash

config_file="TEST_variable_config.txt"

#c1l0RCTTMass5j3b_
#c1l0RCTTMass5j4b_
#c1l1RCTTMass5j3b_
#c1l1RCTTMass5j4b_

prefix_regions="c1l2b_
c1l0RCTTMass8j3b_
c1l0RCTTMass8j4b_
c1l0RCTTMass6_8j3b_
c1l0RCTTMass6_8j4b_
c1l1RCTTMass8j3b_
c1l1RCTTMass8j4b_
c1l1RCTTMass6_8j3b_
c1l1RCTTMass6_8j4b_
c1l2RCTTMass8j3b_
c1l2RCTTMass8j4b_
c1l2RCTTMass6_8j3b_
c1l2RCTTMass6_8j4b_
c1l0RCTTMass5j3b_
c1l0RCTTMass5j4b_
c1l1RCTTMass5j3b_
c1l1RCTTMass5j4b_"

LIST="RCTTMass_jet0_eta
RCTTMass_jet0_m
RCTTMass_jet0_pt
RCTTMass_jet0_consts_n
RCTTMass_jets_eta
RCTTMass_jets_m
RCTTMass_jets_pt
RCTTMass_jets_consts_n
RCTTMass_jets_n
hthad
jets_eta
jets_pt
mbb_mindR
meff
met_zoom
mtbmin_zoom
jets_n
bjets_n
RCjets_eta
RCjets_pt
RCjets_m
RCjet0_eta
RCjet0_pt
RCjet0_m
RCjets_n"

#RCjets_jet0_consts_n
#RCjets_jets_consts_n
#RCjets_nconsts
#bjets_eta
#bjets_pt
#jets_m


#c1l0RCTTMass8j3b_RCjets_n->Integral()

echo "BEGIN">>${config_file}

for f in ${LIST}
do
    
    for reg in ${prefix_regions}
    do
	
	if [ "${reg}" == "c1l2b_" ]
	then 
	    LABEL="Preselection"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass8j3b_" ]
	then 
	    LABEL="0J, #geq 9j, 3b"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass8j4b_" ]
	then 
	    LABEL="0J, #geq 9j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass6_8j3b_" ]
	then 
	    LABEL="0J, [6,8]j, 3b"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass6_8j4b_" ]
	then 
	    LABEL="0J, [6,8], #geq 4b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass8j3b_" ]
	then 
	    LABEL="1J, #geq 9j, 3b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass8j4b_" ]
	then 
	    LABEL="1J, #geq 9j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass6_8j3b_" ]
	then 
	    LABEL="1J, [6,8]j, 3b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass6_8j4b_" ]
	then 
	    LABEL="1J, [6,8]j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l2RCTTMass8j3b_" ]
	then 
	    LABEL="2J, #geq 9j, 3b"
	fi
	
	if [ "${reg}" == "c1l2RCTTMass8j4b_" ]
	then 
	    LABEL="2J, #geq 9j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l2RCTTMass6_8j3b_" ]
	then 
	    LABEL="2J, [6,8]j, 3b"
	fi
	
	if [ "${reg}" == "c1l2RCTTMass6_8j4b_" ]
	then 
	    LABEL="2J, [6,8]j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass5j3b_" ]
	then 
	    LABEL="0J, 5j, 3b"
	fi
	
	if [ "${reg}" == "c1l0RCTTMass5j4b_" ]
	then 
	    LABEL="0J, 5j, #geq 4b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass5j3b_" ]
	then 
	    LABEL="1J, 5j, 3b"
	fi
	
	if [ "${reg}" == "c1l1RCTTMass5j4b_" ]
	then 
	    LABEL="1J, 5j, #geq 4b"
	fi
	
	echo "">>${config_file}
	echo "NEW">>${config_file}
	echo "NAME : ${reg}${f}">>${config_file}
	echo "DRAWSTACK : FALSE">>${config_file}
	echo "EXTRALABEL : #splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{${LABEL}}">>${config_file}
	echo "REBIN : 1">>${config_file}
    done
    
done

echo "">>${config_file}
echo "END">>${config_file}