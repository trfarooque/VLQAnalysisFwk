from ROOT import TFile
import re

import argparse

parser = argparse.ArgumentParser()

parser.add_argument('-i', '--inputFile', help='sample input file from which to read histogram names', required=True)
parser.add_argument('-o', '--outputFile', help='name of output config file', default='variableConfig.txt')
parser.add_argument('-c', '--channel', help='0lep/1lep', required=True)

args = parser.parse_args()

def GenerateRegionLabel(region):
    
    extraLabel = "EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140 fb^{-1}}}"

    # Jet and Boosted Object Multiplicity Regex
    #BOM = re.compile(r"[\d_\d]{1,3}[HVTL]+[a-z]{2,3}")
    JBOMR = re.compile(r"(?P<Multiplicity>[\d_\d]{1,3})(?P<Object>[jfbMJHVTL]+)(?P<Type>[a-z]{2,3})")

    JBOMs = JBOMR.finditer(region)

    labelDict = {
        "lep"  : region[region.find("lep")-1]+"l",
        "jets" : [],
        "BOs"  : [],
        "other": []
    }

    if(region.find("HighMtbmin") != -1):
        labelDict["other"].append("HM")
    elif(region.find("LowMtbmin") != -1):
        labelDict["other"].append("LM")
    elif(region.find("ZwinMLL") != -1):
        labelDict["other"].append("|m_{ll}-M_{Z}| #leq 10GeV")
    elif(region.find("HighMVAScore") != -1):
        labelDict["other"].append("HMVA")
    elif(region.find("MidMVAScore") != -1):
        labelDict["other"].append("MMVA")
    elif(region.find("LowMVAScore") != -1):
        labelDict["other"].append("LMVA")

    for JBO in JBOMs:

        labelComponents = JBO.groupdict()

        tmp_label = ""

        if(labelComponents["Type"] == "in"):
            tmp_label += "#geq"
        
        tmp_label += (labelComponents["Multiplicity"][0]+"-"+labelComponents["Multiplicity"][2]) if (len(labelComponents["Multiplicity"]) == 3) else (labelComponents["Multiplicity"])
            
        if(labelComponents["Object"] == "T"):
            tmp_label += "t_{h}"
        elif(labelComponents["Object"] == "L"):
            tmp_label += "t_{l}"
        elif(labelComponents["Object"] == "VT"):
            tmp_label += ("(V+t_{h})")
        elif(labelComponents["Object"] == "LT"):
            tmp_label += ("(t_{l}+t_{h})")
        elif(labelComponents["Object"] == "VL"):
            tmp_label += ("(V+t_{l})")
        elif(labelComponents["Object"] == "VLT"):
            tmp_label += ("(V+t_{l}+t_{h})")
        else:
            tmp_label += labelComponents["Object"]
        
        if(labelComponents["Object"] in ["j", "b", "fj"]):
            labelDict["jets"].append(tmp_label)
        else:
            labelDict["BOs"].append(tmp_label)

        #print(JBO.groupdict(), JBO.group(), labelDict["lep"])

    
    label = labelDict["lep"] + ", "

    for jets in labelDict["jets"]:
        label += jets + ", "

    for BOs in labelDict["BOs"]:
        label += BOs + ", "

    for other in labelDict["other"]:
        label += other + ", " 
    
    if(label[-2:] == ", "):
        label = label[:-2]


    print(label)

    extraLabel += ("{"+label+"}\n")

    return extraLabel


f = TFile(args.inputFile)


##From MVAHISTS file #####
varList = ['MVAScore', 'meff', 'mtbmin', 'met',
           'jets_n', 'bjets_n', 'RCMTT_jets_n', "RCMHiggs_jets_n", "RCMTop_jets_n", "RCMV_jets_n",
           'mvlq0_RCTTM_drmax', 'mvlq0_RCTTM_drmax', 'mvlq1_rcj_drmax', 'mvlq1_rcj_drmax',
           'RCjet0_pt', 'RCjet1_pt', 'RCjet2_pt', 'RCMTop0_pt', 'RCMHiggs0_pt', 'RCMV0_pt',
           'RCMV0_nconsts', 'RCMV0_nbconsts', 'RCMHiggs0_nconsts', 'RCMHiggs0_nbconsts',
           'dRmin_RCjets', 'dRmin_RCMTT', 'dPhiavg_RCjets', 'dEtaavg_RCjets',
           'dEtamin_RCjets', 'dEtamin_RCMTT', 'leadingdR_RCjets', 'leadingdPhi_RCjets', 'dPhiavg_RCMTTMET']


#varList = ["MVAScore","RCMHiggs0_eta","RCMHiggs0_nbconsts","RCMHiggs0_nconsts","RCMHiggs0_pt","RCMHiggs1_pt","RCMHiggs_jets_n",
#           "RCMTT_jets_n","RCMTop0_eta","RCMTop0_nbconsts","RCMTop0_nconsts","RCMTop0_pt","RCMTop1_pt","RCMTop_jets_n",
#           "RCMV0_eta","RCMV0_nbconsts","RCMV0_nconsts","RCMV0_pt","RCMV1_pt","RCMV_jets_n","RCjet0_pt","RCjet1_pt",
#           "RCjet2_pt","RCjets_n","bjets_n","dEtaavg_RCMTT","dEtaavg_RCjets","dEtamin_RCMTT","dEtamin_RCjets",
#           "dPhiavg_RCMET","dPhiavg_RCMTT","dPhiavg_RCMTTMET","dPhiavg_RCjets","dPhimin_RCMET","dPhimin_RCMTT",
#           "dPhimin_RCMTTMET","dPhimin_RCjets","dRavg_RCMTT","dRavg_RCjets","dRmin_RCMTT","dRmin_RCjets","hthad",
#           "jets_n","leadingdEta_RCMTT","leadingdEta_RCjets","leadingdPhi_RCMET","leadingdPhi_RCMTT","leadingdPhi_RCMTTMET",
#           "leadingdPhi_RCjets","leadingdR_RCMTT","leadingdR_RCjets","leptop_n","leptop_pt","meff","met","mtbmin",
#           "mtw","mvlq0_RCTTM_drmax","mvlq0_rcj_drmax","mvlq1_RCTTM_drmax","mvlq1_rcj_drmax","ptw","residualMET"]
#

rebinDict = {"RCMHiggs_jets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             "RCMTop_jets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             "meff" :"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000,6000,7000",
             "meffred" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,5000,7000",
             "hthad" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "htall" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "htred" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "jets_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "jet0_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "RCMHiggs0_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCMHiggs1_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCMTop0_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCMTop1_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCMV0_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCMV1_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCjet0_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCjet1_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "RCjet2_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "leptop_pt" : "0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000",
             "met" : "0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600",
             "mvlq0_RCTTM_drmax" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000",
             "mvlq1_RCTTM_drmax" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000",
             "mvlq0_rcj_drmax" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000",
             "mvlq1_rcj_drmax" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000",
             "trkbjets_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "trkbjet0_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "RCjets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             "jets_n" : "-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,15.5"}

regList=["c1lep6jin3bin2Min3Jin","c1lep5jin2bin"] if(args.channel=='1lep') else ["c0lep6jin2bin","c0lep6jin2bex", "c0lep6jin3bin", "c0lep6jin2bin2Min"]

varConfig = open(args.outputFile, "w+")

varConfig.write("BEGIN\n")

for var in varList:

    for reg in regList:
        
        fullName = reg+"_"+var
        fullNameWildcard = reg+"_*"+var

        if(f.GetListOfKeys().Contains(fullName)):
            varConfig.write("NEW\n")
            varConfig.write("NAME : "+fullNameWildcard+"\n")
            varConfig.write("DRAWSTACK : TRUE\n")
            varConfig.write("DRAWRES : RATIO\n")
            varConfig.write("DRAWRESSTACK : TRUE\n")
            varConfig.write("DOSCALE : NORM\n")
            varConfig.write("YMIN   : 0.\n")
            #varConfig.write("ISLOGY : TRUE\n")
            varConfig.write("DOWIDTH : FALSE\n")
            varConfig.write("DOCUMULATIVE : BACKWARD\n")
            if(var in rebinDict):
                varConfig.write("REBINVAR : "+rebinDict[var]+"\n")
            

            #pVLQ_regions
            varConfig.write(GenerateRegionLabel(reg))
            varConfig.write("OUTPUTFOLDER  : "+reg+"\n")
            varConfig.write("\n")

        else:
            print("WARNING! Variable "+fullName+" not found in input file!")

f.Close()
varConfig.write("END")
varConfig.close()
