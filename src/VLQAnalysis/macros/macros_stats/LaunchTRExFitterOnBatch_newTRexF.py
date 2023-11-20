#!/bin/python
import os
import glob
import sys
import socket

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Job import *

##----------------------------------------------------------
## Write scripts
##----------------------------------------------------------
def writeScripts( scriptName, configFile, tRexFitterOutDirectory, instructions , jobParams ):

    script = open(scriptName,'w')
    script.write("#!/bin/bash \n")

    script.write("cd $TMPDIR \n\n")

    #script.write("echo \"==> About to copy the TRexFitter tarball !\"\n")
    #script.write("cp -r " + m_tarballPath + " tarball.tgz \n")
    #script.write("echo \"==> After the copy ! (doing an ls just in case)\"\n")
    #script.write("ls -lrth \n")
    #script.write("\n\n")

    #script.write("echo \"==> Now untarring the tarball to have the code !\"\n")
    #script.write("tar xf tarball.tgz \n")
    #script.write("echo \"==> After the copy ! (doing an ls just in case)\"\n")
    #script.write("ls -lrth \n")
    #script.write("\n\n")

    script.write("echo \"==> Setting up the TRexFitter code !\"\n")
    script.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase \n")
    script.write("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh \n")
    script.write("asetup StatAnalysis,0.2.4")
    script.write("\n\n")

    script.write("echo \"==> Now getting the CONFIG file\"\n")
    script.write("cp " + configFile + " configFile.txt \n")
    script.write("echo \"==> Done\"\n")
    script.write("\n\n")

    #script.write("source setup.sh \n")
    #script.write("rm -rf build \n")
    #script.write("mkdir -p build && cd build/ && cmake ../ && cmake --build ./ && cd ../")
    #script.write("\n\n")

    script.write("mkdir -p " + tRexFitterOutDirectory + "/Histograms/\n")

    script.write("echo \"==> Now running the code with the following instructions:\"\n")
    for inst in instructions:
        script.write("echo \"    " + inst + "\"\n")

    if "RANKING" in jobParams["action"]:
        counter = jobParams["counter"]
    else:
        counter = 0
    script.write("trex-fitter h configFile.txt >& logFileRunning_h \n")
    for inst in instructions:
        script.write("echo \"==> Now executing the instruction: " + inst + "\"\n")
        script.write("trex-fitter " + inst.replace("_CONFIGFILE_","configFile.txt") + " >& logFileRunning_" + str(counter) + " \n")
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
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Pulls\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Plots\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Significance\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Tables\n")
    script.write("mkdir -p " + m_outputDir + "/" + tRexFitterOutDirectory + "/Limits\n")

    #Copying outputs in the final repository
    script.write("mv " + tRexFitterOutDirectory + "/Systematics/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Systematics \n")
    script.write("mv " + tRexFitterOutDirectory + "/*.{png,eps,pdf} " + m_outputDir + "/" + tRexFitterOutDirectory + "/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Fits/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Fits/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/RooStats/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/RooStats/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Pulls/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Pulls/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Plots/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Plots/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Significance/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Significance/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Tables/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Tables/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Limits/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Limits/ \n")
    # script.write("mv " + tRexFitterOutDirectory + "/Histograms/* " + m_outputDir + "/" + tRexFitterOutDirectory + "/Histograms/ \n")
    script.write("mv logFileRunning* " + m_outputDir + "/" + tRexFitterOutDirectory + "/ \n")

    script.write("\n\n")
    script.write("echo \"==> Finished transfering the output files !\"\n")
    script.write("\n\n")
 
    if m_runPBS:
        script.write("rm -rf $TMPDIR/*")
    script.close()

    if not m_runPBS:
        writeCondorSubmitScript(scriptName, tRexFitterOutDirectory, jobParams )

##_________________________________________________________________________
##
def writeCondorSubmitScript(scriptName, tRexFitterOutDirectory, jobParams ):

    f = open(scriptName+".sub","w")
    f.write("#Basic configuration \n")
    f.write("executable              = "+scriptName+" \n")
    f.write("output                  = "+"trexf.$(ClusterId).$(ProcId).out \n")
    f.write("error                   = "+"trexf.$(ClusterId).$(ProcId).err \n")
    f.write("log                     = "+"trexf.$(ClusterId).log \n")
    f.write("\n")
    f.write("\n")
    f.write("#Duration of job \n")
    f.write("+IsMediumJob = true")
    f.write("\n")
    f.write("\n")
    f.write("request_memory           = %i GB \n"%jobParams["memory"]) # default: 2 GB
    f.write("request_cpus             = %i \n"%jobParams["cpus"])     # default: 1
    f.write("request_disk             = %i GB \n"%jobParams["disk"]) # default: 15 GB
    f.write("#Script options \n")
    f.write("nJobs                   = 1 \n")
    f.write("\n")
    f.write("\n")
    #f.write("include command : mkdir "+self.logDir+"/output \n")
    #f.write("include command : mkdir "+self.logDir+"/error \n")
    #f.write("include command : mkdir "+self.logDir+"/log \n")
    f.write("#Queue subjobs \n")
    f.write("queue $(nJobs) \n")
    f.close()
    os.system("chmod +x "+scriptName+".sub")

##----------------------------------------------------------
## Launch jobs
##----------------------------------------------------------
def LaunchJobs( scriptName, tRexFitterOutDirectory ):

    os.system("mkdir " + m_outputDir + "/" + tRexFitterOutDirectory)
    os.system("chmod +x " + m_outputDir + "/*")
    com=""
    if m_runPBS:
        com="qsub -q " + m_batch_queue + " " + scriptName 
        com+= " -o " + m_outputDir + "/" + tRexFitterOutDirectory 
        com+= "/ -e " + m_outputDir + "/" + tRexFitterOutDirectory + "/" 
    else:
        com="condor_submit " + scriptName + ".sub"

    if m_dryRun:
        print(" Command to launch: " + com)
    else:
        if m_verbose:
            print(com)
        os.system(com)


##----------------------------------------------------------
## Write scripts and launch jobs
##----------------------------------------------------------
def writeScriptsAndLaunchJobs( scriptTempName, configFile, instructions , jobParams ):
    pathToCreate = ""
    scriptName = scriptTempName

    for level in scriptName.split("/"):
        if(level==scriptName.split("/")[len(scriptName.split("/"))-1]): continue
        pathToCreate += level
        pathToCreate += "/"
    os.system("mkdir -p " + pathToCreate)
    os.system("chmod +x " + pathToCreate + "/*")

    scriptName += configFile.split("/")[len(configFile.split("/"))-1].replace(" ","")

    tRexFitterOutDirectory = ""
    f_config_file = open(configFile,'r')
    for config_line in f_config_file:
        if config_line.find("Job:")>-1:
            config_line_splitted = config_line.split(" ")
            if len(config_line_splitted)>1:
                tRexFitterOutDirectory = config_line_splitted[1].replace("\"","").replace("\n","")
                break

    writeScripts(scriptName, configFile, tRexFitterOutDirectory, instructions, jobParams )
    LaunchJobs(scriptName, tRexFitterOutDirectory)


##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------
if(len(sys.argv)<4):
    printWarning("Arguments checker ==> Wrong input arguments")
    print("")
    print("    python "+sys.argv[0]+" [arg]")
    print("")
    print("Arguments")
    print("=========")
    #print("    tarball=<path to the tarball containing TRExFitter>")
    print("    outputDir=<place where to store all the results>")
    print("    action=<action to perform with this job> chosen between RANKING/HISTOS/FIT/POSTPLOTS/PREPLOTS/LIMIT")
    print("    inputDir=<ABSOLUTE path to the config files>")
    print("    n_NP=<number of NP to be run for each job when performing the ranking> ")
    print("    queue=<name of the batch queue to launch the jobs. delfault in at3> ")
    print("    dryrun : No jobs submitted ")
    print("    runPBS : Launch jobs on PBS queue [default: condor] ")
    print("    verbose : print(launch command for each job ")
    print("")
    sys.exit()

##------------------------------------------------------
## Selects the arguments
##------------------------------------------------------
#m_tarballPath = "tarball.tgz"
m_outputDir = ""
m_action = ""
m_inputConfigFiles = ""
m_rankingNPMerging = 10
m_mergeJobs = False
m_dryRun = False
m_runPBS = False
m_batch_queue = "at3"
m_verbose = False
m_memory = 16
m_cpus = 4
m_disk = 15

for iArg in range(1,len(sys.argv)):
    splitted=sys.argv[iArg].split("=")
    argument = splitted[0].upper()
    #if(argument=="TARBALL"): m_tarballPath = splitted[1]
    if(argument=="OUTPUTDIR"): m_outputDir = splitted[1]
    elif(argument=="ACTION"): m_action = splitted[1].upper()
    elif(argument=="INPUTDIR"): m_inputConfigFiles = splitted[1]
    elif(argument=="N_NP"): m_rankingNPMerging = int(splitted[1])
    elif(argument=="QUEUE"): m_batch_queue = splitted[1]
    elif(argument=="MERGEJOBS"): m_mergeJobs = True
    elif(argument=="RUNPBS"): m_runPBS = True
    elif(argument=="DRYRUN"): m_dryRun = True
    elif(argument=="VERBOSE"): m_verbose = True
    elif(argument=="MEMORY"): m_memory = int(splitted[1])
    elif(argument=="CPUS"): m_cpus = int(splitted[1])
    elif(argument=="DISK"): m_disk = int(splitted[1])
    else:
        printWarning("/!\ Unrecognized argument ("+splitted[0]+") ! Please check !")
if(m_inputConfigFiles==""):
    printError("<!> Please provide an input config file to use !")
    sys.exit()

##------------------------------------------------------
## Setting the job parameters
##------------------------------------------------------
jobParams = {"memory": m_memory,
             "cpus":   m_cpus,
             "disk":   m_disk ,
             "action": m_action}

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
    printWarning("=> You are about to submit A LOT of jobs ("+str(len(configFileList))+") ... Are you sure you want to do that ?")
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
    print( "=> Taking care of config file: ", config)
    if m_action == "RANKING":
        #Determining the number of systematics to consider
        f_config = open(config,'r')
        syst_list = []
        for config_line in f_config:
            if config_line.find("Systematic: ")>-1 and not "#" in config_line and not "%" in config_line:
                temp_line = config_line
                temp_line = temp_line.replace("Systematic: ","").replace("\"","")
                syst_list += temp_line.split(";")
        f_config.close()

        #Splits into subjobs to maximize efficiency ...
        commands = []
        for n_syst,systematic in enumerate(syst_list):
            syst = systematic.replace("\"","").replace(" ","").replace("\n","")
            if commands == []:
                commands += ["wfr _CONFIGFILE_ 'Ranking=" + syst + "'"]
            else:
                commands += ["r _CONFIGFILE_ 'Ranking=" + syst + "'"]
            if len(commands)==m_rankingNPMerging:
                jobParams["counter"] = n_syst - m_rankingNPMerging + 1
                writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING"+syst, config, commands , jobParams )
                commands = []
            elif commands != [] and n_syst == len(syst_list)-1:
                jobParams["counter"] = n_syst - len(syst_list)%m_rankingNPMerging + 1
                writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING"+syst, config, commands , jobParams )
                jobParams["counter"] = len(syst_list)
                commands = []

    if m_action == "RANKING_GAMMAS":
        #Finding the gammas to run over
        f_config = open(config,'r')
        gam_list = []
        sig_index = open(config,'r').read().split('\n').index('  Type: signal') - 2
        sig_name = open(config,'r').read().split('\n')[sig_index].replace('Sample: ','').replace('"','')
        for line_number,config_line in enumerate(f_config):
            if config_line.find("Region: HTX")>-1 and not "VR" in config_line and not "#" in config_line and not "%" in config_line:
                reg_line = config_line
                reg_line = reg_line.replace("Region: ","").replace("\n","")
                bin_line = [line for line in open(config,'r').read().split('\n')[line_number:] if "Binning: " in line][0]
                bin_line = bin_line.replace("Binning: ","").replace("\"","")
                bin_length = len(map(int, bin_line.split(',')))

                for bin_i in range(bin_length-2):
                    gam_list.append('gamma_stat_%s_bin_%s'%(reg_line,bin_i))
                    gam_list.append('gamma_shape_stat_%s_%s_bin_%s'%(sig_name,reg_line,bin_i))
        f_config.close()

        #Splits into subjobs to maximize efficiency ...
        commands = []
        for n_gam,gamma in enumerate(gam_list):
            if commands == []:
                commands += ["wfr _CONFIGFILE_ 'Ranking=" + gamma + "'"]
            else:
                commands += ["r _CONFIGFILE_ 'Ranking=" + gamma + "'"]
            if len(commands)==m_rankingNPMerging:
                jobParams["counter"] = n_gam - m_rankingNPMerging + 1
                writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING_GAMMAS_"+gamma, config, commands , jobParams )
                commands = []
            elif commands != [] and n_gam == len(gam_list)-1:
                jobParams["counter"] = n_gam - len(gam_list)%m_rankingNPMerging + 1
                writeScriptsAndLaunchJobs( m_outputDir + "/scripts_RANKING_GAMMAS_"+gamma, config, commands , jobParams )
                jobParams["counter"] = len(gam_list)
                commands = []
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
        writeScriptsAndLaunchJobs( m_outputDir + "/scripts_", config, [com + " _CONFIGFILE_",] , jobParams )
#        writeScriptsAndLaunchJobs( m_outputDir + "/scripts_", config, [com + " _CONFIGFILE_",], m_mergeJobs==False , jobParams )

if m_mergeJobs:
    list_scripts = glob.glob(m_outputDir + "/scripts_*")
    counter = 0
    counter_scripts = 0
    for script in list_scripts:
        if counter == 0:
            counter_scripts += 1
            counter += 1
            os.system("cat " + script + " > " + m_outputDir + "/mergedScripts_"+str(counter_scripts))
        else:
            counter += 1
            os.system("echo '\n\n\n\n' >> " + m_outputDir + "/mergedScripts_"+str(counter_scripts))
            os.system("cat " + script + " >> " + m_outputDir + "/mergedScripts_"+str(counter_scripts))

        if counter==nMerge:
            counter = 0
            com = "qsub -q at3 " + m_outputDir + "/mergedScripts_"+str(counter_scripts) + " -o " + m_outputDir + "/LOG_mergedScripts_"+str(counter_scripts) + "/ -e " + m_outputDir + "/LOG_mergedScripts_"+str(counter_scripts) + "/"
            os.system(com)
    printGoodNews("=> You effectively submitted \"only\" " + str(counter_scripts) + " jobs instead of "+str(len(configFileList))+" !!")
