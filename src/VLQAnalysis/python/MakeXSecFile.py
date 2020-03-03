#!/bin/python
import sys
import os
import string
import time, getpass
import socket
import datetime
import json
#from ROOT import *
from VLQ_Samples_mc import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Job import *
from Samples import *


##________________________________________________________
## OPTIONS

listProc=[]
xSecFilePath=""
procDir=""
debug=True
campaign="mc16a"
##........................................................

if(len(sys.argv))>1:
    for x in sys.argv[1:]:
        splitted=x.split("=")
        if(len(splitted)!=2):
            printError("<!> The argument \"" + x + "\" has no equal signs ... Please check")
            sys.exit(-1)
        argument = splitted[0].upper().replace("--","")
        value = splitted[1]
        if(argument=="LISTPROC"):
            listProc=value.split(",")
        elif(argument=="XSECFILE"):
            xSecFilePath=value
        elif(argument=="PROCDIR"):
            procDir=value
        elif(argument=="DEBUG"):
            debug=(value.upper()=="TRUE")
        else:
            printError("Unknown argument : "+argument)
else:
    printError("<!> No arguments seen ... Aborting !")
    sys.exit(-1)


##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
listFolder=here+"/Lists_Analysis_" + now
##........................................................

##________________________________________________________
## Creating useful repositories
os.system("mkdir -p " + listFolder) #list files folder
##........................................................

# Merge all provided processes for each mc campaign into one text file and run metaData query
# Then, open the output file and read in the xsec for each DSID
sampleMetaList = {}
sortedFiles = {}
DSset = set()



metaFilePath=listFolder+"/XSec_tag21.2.67-htx-0.list"
sampleMetaList = {}
sortedFiles = {}

DSlist=listFolder+"/DSlist_tag21.2.67-htx-0.list"
        
with open(DSlist, 'w') as DSfile:
    
    for proc in listProc:
        
        dspath=procDir+'/'+campaign+'/'+campaign+"."+proc+'.list'
        with open(dspath) as procdsfile:
            for line in procdsfile:
                if not line.startswith("mc16_13TeV"):
                    continue
                DSfile.write(line)


com="getMetadata.py --inDsTxt="
com+=DSlist
com+=" --outFile="
com+=metaFilePath

print "Calling " + com    

result=os.system(com)
print "After metadata query"



#convert metaFile into a python dictionary
with open(metaFilePath) as metaFile:
    for line in metaFile:
        
        #remove whitespace
        line=line.strip()
        #remove empty line
        if(len(line)==0):
            continue
        #first column is DSID
        #multiply next three columns
        if("dataset_number/I:crossSection/D:kFactor/D:genFiltEff/D" in line):
            continue
        if(line.startswith("#")):
            continue
        if(len(line.split())!=4):
            printWarning(" Ouput of getMetadata has unexpected format. Please check line \n "+line)
            continue
        dsid=line.split()[0]

        try:
            xsec = float(line.split()[1])*1000.
        except:
            printWarning(" WARNING: dsid "+dsid+" has xsec "+line.split()[1]+". \n Setting to -1.")
            xsec = -1.

        try:
            kFactor = float(line.split()[2])
        except:
            printWarning(" WARNING: dsid "+dsid+" has kFactor "+line.split()[1]+". \n Setting to -1.")
            kFactor = -1.

        try:
            genFiltEff = float(line.split()[3])
        except:
            printWarning(" WARNING: dsid "+dsid+" has genFiltEff "+line.split()[1]+". \n Setting to -1.")
            genFiltEff = -1.

        DSset.add(dsid)
        sampleMeta = { "dsid": line.split()[0],
                       "xsec": xsec,
                       "kFactor": kFactor,
                       "genFiltEff": genFiltEff
        }
        sampleMetaList[dsid] = sampleMeta

with open(xSecFilePath, 'w') as outfile:
    print json.dump(sampleMetaList, outfile, indent=4, separators=(',', ': '))
print " All xsec written "

