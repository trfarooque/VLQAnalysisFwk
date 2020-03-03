#!/bin/python
import glob
import os,sys

#path_full_sim="/nfs/pic.es/user/l/lvalery/scratch2/VLQAnalysisRun2/VLQAnalysisOutputs_SingleTopSyst_FullSim_2017_05_27_1946/"
#path_af2="/nfs/pic.es/user/l/lvalery/scratch2/VLQAnalysisRun2/VLQAnalysisOutputs_SingleTopSyst_AF2_2017_05_27_1946/"

path_full_sim="/nfs/at3/scratch2/lvalery/VLQAnalysisRun2/VLQAnalysisOutputs_SingleTopSyst_FullSim_2017_05_27_1946/"
path_af2="/nfs/at3/scratch2/lvalery/VLQAnalysisRun2/VLQAnalysisOutputs_SingleTopSyst_AF2_2017_05_27_1946/"

systematics = []
#Wt uncertainties
#systematics += [{'name':"WtchanradHi",'simulation':"AF2",'systematics':"*Wt*radHi*.root"}]
#systematics += [{'name':"WtchanradLo",'simulation':"AF2",'systematics':"*Wt*radLo*.root"}]
#systematics += [{'name':"WtchanPS",'simulation':"AF2",'systematics':"*Wt*PowHer*.root"}]
systematics += [{'name':"WtchanDS",'simulation':"FullSim",'systematics':"*WtDiag*.root"}]
#t uncertainties
#systematics += [{'name':"tchanradHi",'simulation':"AF2",'systematics':"*tchanradHi*.root"}]
#systematics += [{'name':"tchanradLo",'simulation':"AF2",'systematics':"*tchanradLo*.root"}]
#systematics += [{'name':"tchanPS",'simulation':"AF2",'systematics':"*tchanPowHpp*.root"}]
#systematics += [{'name':"tchanGen",'simulation':"AF2",'systematics':"*tchanaMCaNLOHpp*.root"}]
#s uncertainties
#systematics += [{'name':"schanradHi",'simulation':"AF2",'systematics':"*schanradHi*.root"}]
#systematics += [{'name':"schanradLo",'simulation':"AF2",'systematics':"*schanradLo*.root"}]

nominal_samples = []
nominal_samples += [{'name':"tchan",'dsids':"410011,410012"}]
nominal_samples += [{'name':"Wtchan",'dsids':"410013,410014"}]
nominal_samples += [{'name':"schan",'dsids':"410025,410026"}]

command_af2 = "hadd Nominal_AF2.root "
command_full_sim = "hadd Nominal_FullSim.root "
for sample in nominal_samples:
    for subsample in sample['dsids'].split(","):
        for sample_file in glob.glob(path_af2 + "/out*" + subsample + "*.root" ):
            command_af2 += sample_file
            command_af2 += " "
        for sample_file in glob.glob(path_full_sim + "/out*" + subsample + "*.root" ):
            command_full_sim += sample_file
            command_full_sim += " "
os.system(command_full_sim)
os.system(command_af2)

for sys in systematics:
    affected_component = ""
    if(sys['name'].find("Wtchan")>-1):
        affected_component = "Wtchan"
    elif(sys['name'].find("schan")>-1):
        affected_component = "schan"
    elif(sys['name'].find("tchan")>-1):
        affected_component = "tchan"

    command = "hadd "
    command += sys['name']
    command += "_" + sys['simulation']
    command += ".root "

    path_to_files = path_af2
    if sys['simulation']=="FullSim":
        path_to_files = path_full_sim

    #Adding the components that are not affected by the current systematics
    for sample in nominal_samples:
        if(sample['name']==affected_component):
            continue
        for subsample in sample['dsids'].split(","):
            list_files = glob.glob(path_to_files+"/out*"+subsample+"*.root")
            if(len(list_files)!=1):
                print "==> Error for " + sample['name'] + ": more than one file matching !!"
                continue
            command += list_files[0]
            command += " "

    #Now adding the systematic component
    systematics_files = glob.glob(path_to_files+"/"+sys['systematics'])
    for sf in systematics_files:
        command += sf
        command += " "
    os.system(command)
