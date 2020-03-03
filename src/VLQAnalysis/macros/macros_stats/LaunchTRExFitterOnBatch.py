#!/bin/python
import os
import glob
import sys
import socket

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *

##----------------------------------------------------------
## Write scripts and launch jobs
##----------------------------------------------------------
def writeScriptsAndLaunchJobs( scriptTempName, configFile, instructions, LaunchJobs = True ):
    pathToCreate = ""
    scriptName = scriptTempName

    for level in scriptName.split("/"):
        if(level==scriptName.split("/")[len(scriptName.split("/"))-1]): continue
        pathToCreate += level
        pathToCreate += "/"
    os.system("mkdir -p " + pathToCreate)

    scriptName += configFile.split("/")[len(configFile.split("/"))-1].replace(" ","")
    
    script = open(scriptName,'w')
    script.write("#!/bin/bash \n")

    script.write("cd $TMPDIR \n\n")

    script.write("echo \"==> About to copy the TRexFitter tarball !\"\n")
    script.write("cp -r " + m_tarballPath + " tarball.tgz \n")
    script.write("echo \"==> After the copy ! (doing an ls just in case)\"\n")
    script.write("ls -lrth \n")
    script.write("\n\n")

    script.write("echo \"==> Now untarring the tarball to have the code !\"\n")
    script.write("tar xf tarball.tgz \n")
    script.write("echo \"==> After the copy ! (doing an ls just in case)\"\n")
    script.write("ls -lrth \n")
    script.write("\n\n")

    script.write("echo \"==> Now getting the CONFIG file\"\n")
    script.write("cp " + configFile + " configFile.txt \n")
    script.write("echo \"==> Done\"\n")
    script.write("\n\n")

    script.write("source setup.sh")

    script.write("\n\n")

    tRexFitterOutDirectory = ""
    f_config_file = open(configFile,'r')
    for config_line in f_config_file:
        if config_line.find("Job:")>-1:
            config_line_splitted = config_line.split(" ")
            if len(config_line_splitted)>1:
                tRexFitterOutDirectory = config_line_splitted[1].replace("\"","").replace("\n","")
                break

    script.write("mkdir -p " + tRexFitterOutDirectory + "/Histograms/\n")

    script.write("echo \"==> Now running the code with the following instructions:\"\n")
    for inst in instructions:
        script.write("echo \"    " + inst + "\"\n")

    counter = 0
    script.write("./myFit.exe h configFile.txt >& logFileRunning_h \n")
    for inst in instructions:
        script.write("echo \"==> Now executing the instruction: " + inst + "\"\n")
        script.write("./myFit.exe " + inst.replace("_CONFIGFILE_","configFile.txt") + " >& logFileRunning_" + `counter` + " \n")
        counter += 1

    script.write("\n\n")
    script.write("echo \"==> The code ran ! Doing an ls so that you can check !\"\n")
    script.write("ls -lrth . " + tRexFitterOutDirectory + "/ \n")
    script.write("\n\n")

    #Creating the output repositories
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Histograms\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Systematics\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Fits\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/RooStats\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Plots\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Significance\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Tables\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Limits\n")

    #Copying outputs in the final repository
    script.write("mv " + tRexFitterOutDirectory + "/Systematics/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Systematics \n")
    script.write("mv " + tRexFitterOutDirectory + "/*.{png,eps,pdf} " + m_outputDir + "/" + tRexFitterOutDirectory + "/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Fits/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Fits/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/RooStats/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/RooStats/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Plots/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Plots/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Significance/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Significance/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Tables/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Tables/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Limits/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Limits/ \n")
    script.write("mv logFileRunning* " + m_outputDir + "/" + tRexFitterOutDirectory + "/ \n")

    script.write("\n\n")
    script.write("echo \"==> Finished transfering the output files !\"\n")
    script.write("\n\n")

    script.write("rm -rf $TMPDIR/*")

    script.close()
    if LaunchJobs:
        if socket.gethostname().find("cca")>-1:
            #You are working at CCIN2P3 ... you must be Loic !!
            os.system("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory)
            os.system("qsub -l sps=1 -P P_atlas -o " + m_outputDir + "/" + tRexFitterOutDirectory + "/ -e " + m_outputDir + "/" + tRexFitterOutDirectory + "/ " + scriptName)
        else:
            os.system("mkdir " + m_outputDir + "/" + tRexFitterOutDirectory)
            os.system("qsub -q " + m_batch_queue + " " + scriptName + " -o " + m_outputDir + "/" + tRexFitterOutDirectory + "/ -e " + m_outputDir + "/" + tRexFitterOutDirectory + "/")

##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------
if(len(sys.argv)<5):
    printWarning("Arguments checker ==> Wrong input arguments")
    print ""
    print "    python "+sys.argv[0]+" [arg]"
    print ""
    print "Arguments"
    print "========="
    print "    tarball=<path to the tarball containing TRExFitter>"
    print "    outputDir=<place where to store all the results>"
    print "    action=<action to perform with this job> chosen between RANKING/HISTOS/FIT/POSTPLOTS/PREPLOTS/LIMIT"
    print "    inputDir=<ABSOLUTE path to the config files>"
    print "    n_NP=<number of NP to be run for each job when performing the ranking> "
    print "    queue=<name of the batch queue to launch the jobs. delfault in at3> "
    print ""
    sys.exit()

##------------------------------------------------------
## Selects the arguments
##------------------------------------------------------
m_tarballPath = "tarball.tgz"
m_outputDir = ""
m_action = ""
m_inputConfigFiles = ""
m_rankingNPMerging = 10
m_mergeJobs = False
m_batch_queue = "at3"
for iArg in range(1,len(sys.argv)):
    splitted=sys.argv[iArg].split("=")
    argument = splitted[0].upper()
    if(argument=="TARBALL"): m_tarballPath = splitted[1]
    elif(argument=="OUTPUTDIR"): m_outputDir = splitted[1]
    elif(argument=="ACTION"): m_action = splitted[1].upper()
    elif(argument=="INPUTDIR"): m_inputConfigFiles = splitted[1]
    elif(argument=="N_NP"): m_rankingNPMerging = int(splitted[1])
    elif(argument=="QUEUE"): m_batch_queue = splitted[1]
    elif(argument=="MERGEJOBS"): m_mergeJobs = True
    else:
        printWarning("/!\ Unrecognized argument ("+splitted[0]+") ! Please check !")
if(m_inputConfigFiles==""):
    printError("<!> Please provide an input config file to use !")
    sys.exit()

##------------------------------------------------------
## Creating the output folder
##------------------------------------------------------
os.system("mkdir -p " + m_outputDir)

##------------------------------------------------------
## Getting the file list
##------------------------------------------------------
configFileList = glob.glob(m_inputConfigFiles+"*.txt")
nMerge = 1
if len(configFileList)>100 and not m_mergeJobs:
    printWarning("=> You are about to submit A LOT of jobs ("+`len(configFileList)`+") ... Are you sure you want to do that ?")
    go_ahead=""
    while go_ahead!="y" and go_ahead!="n":
        go_ahead = raw_input("   Type 'y' to continue or 'n' to merge jobs: ")
    if go_ahead=="n":
        m_mergeJobs = True
        nMerge = int(raw_input("   Type the number of jobs to merge: "))

##------------------------------------------------------
## Looping on the configfiles and launch a job (or more for each)
##------------------------------------------------------

for config in configFileList:
    print "=> Taking care of config file: ", config
    if m_action == "RANKING":
        #Determining the number of systematics to consider
        f_config = open(config,'r')
        syst_list = []
        for config_line in f_config:
            if config_line.find("Systematic: ")>-1:
                temp_line = config_line
                temp_line = temp_line.replace("Systematic: ","").replace("\"","")
                syst_list += temp_line.split(";")
        f_config.close()

        #Splits into subjobs to maximize efficiency ...
        commands = []
        for systematic in syst_list:
            syst = systematic.replace("\"","").replace(" ","").replace("\n","")
            if commands == []:
                commands += ["wfr _CONFIGFILE_ 'Ranking=" + syst + "'"]
            else:
                commands += ["r _CONFIGFILE_ 'Ranking=" + syst + "'"]
            if len(commands)==m_rankingNPMerging:
                writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING"+syst, config, commands )
                commands = []
        writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING_ttbbNorm", config, ["wfr _CONFIGFILE_ 'Ranking=HTX_BKGNORM_TTBARBB'",] )
    else:
        com = "w"
        if m_action.find("FIT")>-1:
            com += "f"
            if m_action.find("POSTPLOTS")>-1:
                com += "p"
        if m_action.find("LIMIT")>-1: 
            com += "ls"
        if m_action.find("PREPLOTS")>-1: 
            com += "d"
        writeScriptsAndLaunchJobs( m_outputDir + "/scripts_", config, [com + " _CONFIGFILE_",], m_mergeJobs==False )

if m_mergeJobs:
    list_scripts = glob.glob(m_outputDir + "/scripts_*")
    counter = 0
    counter_scripts = 0
    for script in list_scripts:
        if counter == 0:
            counter_scripts += 1
            counter += 1
            os.system("cat " + script + " > " + m_outputDir + "/mergedScripts_"+`counter_scripts`)
        else:
            counter += 1
            os.system("echo '\n\n\n\n' >> " + m_outputDir + "/mergedScripts_"+`counter_scripts`)
            os.system("cat " + script + " >> " + m_outputDir + "/mergedScripts_"+`counter_scripts`)

        if counter==nMerge:
            counter = 0
            com = "qsub -q at3 " + m_outputDir + "/mergedScripts_"+`counter_scripts` + " -o " + m_outputDir + "/LOG_mergedScripts_"+`counter_scripts` + "/ -e " + m_outputDir + "/LOG_mergedScripts_"+`counter_scripts` + "/"
            os.system(com)
    printGoodNews("=> You effectively submitted \"only\" " + `counter_scripts` + " jobs instead of "+`len(configFileList)`+" !!")
