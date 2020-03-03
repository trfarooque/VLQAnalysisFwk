#!/bin/python

import os,sys
import socket

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from variables_dict import *

'''
Utility to dump the config files to draw the pre-fit and post-fit
distribution of other variables given a postfit NP text files.
'''
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-r", "--regions_dictionary", dest="path_regs", help="path to the regions dictionary")
parser.add_option("-v", "--variables_dictionary", dest="path_vars", help="path to the variable dictionary")
parser.add_option("-o", "--outDir", dest="outDir",
                    help="folder to store the output config files (will be created if not already there)")
parser.add_option("-C", "--template_config", dest="template_config",
                    help="path to the template config file")
parser.add_option("-i", "--input_folder", dest="input_folder",
                    help="path to the input files")
parser.add_option("-f", "--input_fit", dest="input_fit",
                    help="path to the input fit result")
parser.add_option("-t", "--trf_tarball", dest="m_tarballPath",
                    help="path to the TREXFitter tarball",default="/nfs/pic.es/user/l/lvalery/scratch2/TtHFitter-00-03-14/TtHFitter_Binned.tgz")
(options, args) = parser.parse_args()
outputDir = options.outDir
template_config = options.template_config
input_fit = options.input_fit
m_tarballPath = options.m_tarballPath
#m_batch_queue = "at3"
m_batch_queue = "at3_short"
os.system("mkdir -p "+outputDir)

##______________________________________________________________________________
##
def writeScriptsAndLaunchJobs( scriptTempName, configFile, label, LaunchJobs = True ):
    pathToCreate = ""
    scriptName = scriptTempName

    for level in scriptName.split("/"):
        if(level==scriptName.split("/")[len(scriptName.split("/"))-1]): continue
        pathToCreate += level
        pathToCreate += "/"
    os.system("mkdir -p " + pathToCreate)

    #scriptName += configFile.split("/")[len(configFile.split("/"))-1].replace(" ","")

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
    script.write("mkdir -p " + tRexFitterOutDirectory + "/Fits/\n")
    script.write("cp " + input_fit + " " + tRexFitterOutDirectory + "/Fits/" + tRexFitterOutDirectory + ".txt\n")

    script.write("echo \"==> Now running the code with the following instructions:\"\n")

    script.write("./myFit.exe h configFile.txt >& logFileRunning_h \n")
    script.write("./myFit.exe dp configFile.txt  >& logFileRunning_dp \n")

    script.write("\n\n")
    script.write("echo \"==> The code ran ! Doing an ls so that you can check !\"\n")
    script.write("ls -lrth . " + tRexFitterOutDirectory + "/ \n")
    script.write("\n\n")

    #Creating the output repositories
    script.write("mkdir -p " + outputDir + "/" + tRexFitterOutDirectory + "/Plots\n")
    script.write("mkdir -p " + outputDir + "/" + tRexFitterOutDirectory + "/Tables\n")
    script.write("mkdir -p " + outputDir + "/" + tRexFitterOutDirectory + "/Systematics\n")
    script.write("mkdir -p " + outputDir + "/" + tRexFitterOutDirectory + "/Histograms\n")

    #Copying outputs in the final repository
    script.write("mv " + tRexFitterOutDirectory + "/Plots/* " + outputDir + "/" + tRexFitterOutDirectory + "/Plots/ \n")

    script.write("mv " + tRexFitterOutDirectory + "/Tables/Yields.txt " + outputDir + "/" + tRexFitterOutDirectory + "/Tables/Yields_"+label+".txt \n")
    script.write("mv " + tRexFitterOutDirectory + "/Tables/Yields_postFit.txt " + outputDir + "/" + tRexFitterOutDirectory + "/Tables/Yields_"+label+"_postFit.txt \n")
    script.write("mv " + tRexFitterOutDirectory + "/Tables/* " + outputDir + "/" + tRexFitterOutDirectory + "/Tables/ \n")
    script.write("mv " + tRexFitterOutDirectory + "/Histograms/* " + outputDir + "/" + tRexFitterOutDirectory + "/Histograms/ \n")
    #script.write("mv " + tRexFitterOutDirectory + "/Systematics/* " + outputDir + "/" + tRexFitterOutDirectory + "/Systematics/ \n")
    script.write("rsync -arv " + tRexFitterOutDirectory + "/Systematics/* " + outputDir + "/" + tRexFitterOutDirectory + "/Systematics/. \n")
    script.write("mv logFileRunning* " + outputDir + "/" + tRexFitterOutDirectory + "/ \n")

    script.write("\n\n")
    script.write("echo \"==> Finished transfering the output files !\"\n")
    script.write("\n\n")

    script.write("rm -rf $TMPDIR/*")

    script.close()
    if LaunchJobs:
        if socket.gethostname().find("cca")>-1:
            #You are working at CCIN2P3 ... you must be Loic !!
            os.system("mkdir -p " + outputDir + "/" + tRexFitterOutDirectory)
            os.system("qsub -l sps=1 -P P_atlas -o " + outputDir + "/" + tRexFitterOutDirectory + "/ -e " + outputDir + "/" + tRexFitterOutDirectory + "/ " + scriptName)
        else:
            os.system("mkdir " + outputDir + "/" + tRexFitterOutDirectory)
            os.system("qsub -q " + m_batch_queue + " " + scriptName + " -o " + outputDir + "/" + tRexFitterOutDirectory + "/ -e " + outputDir + "/" + tRexFitterOutDirectory + "/")


##______________________________________________________________________________
##
lepchannels = [""]#,"_el","_mu"]

for reg in regions:
    for var in list_variables:
        if 'reg' in var.keys():
            if not reg['name'] in var['reg']:
                continue
        for channel in lepchannels:
            histoName = reg['name'].replace("HTX_", "") + channel + "_" + var['name']
            f_template = open(template_config,'r')
            temp_config = template_config.replace("_VAR_",histoName)
            f_new = open(outputDir+"/"+temp_config,'w')
            for old_line in f_template:
                new_line = old_line
                if( new_line.find("_INPUTDIR_")>-1 ):
                    new_line = old_line.replace("_INPUTDIR_",options.input_folder)
                elif( new_line.find("_REGIONS_")>-1 ):
                    new_line = "\n"
                    new_line += "Region: " + histoName + "\n"
                    new_line += "Type: VALIDATION" + "\n"
                    new_line += "HistoName: " + histoName + "\n"
                    new_line += "VariableTitle: " + var['legend'] + "\n"
                    if 'binning' in var.keys():
                        new_line += "Binning: " + var['binning'] + "\n"
                    if 'width' in var.keys():
                        new_line += "BinWidth: " + var['width'] + "\n"
                    new_line += "Label: " + reg['legend'] + "\n"
                    new_line += "LogScale: TRUE\n"
                    new_line += "ShortLabel: " + reg['legend'] + "\n"
                    if reg['name'].find("1lep")>-1:
                        new_line += "HistoPathSuff: 1lep/" + "\n"
                    else:
                        new_line += "HistoPathSuff: 0lep/" + "\n"
                    new_line += "\n"
                f_new.write(new_line)
            f_new.close()
            print("Launching ",outputDir+"/script_"+temp_config)
            writeScriptsAndLaunchJobs( outputDir+"/script_"+temp_config, outputDir+"/"+temp_config, reg['name']+"_"+var['name'], LaunchJobs = True )
##..............................................................................
##
