from ROOT import TFile
import re

def GenerateRegionLabel(region):
    
    extraLabel = "EXTRALABEL : #splitline{#splitline{#scale[1.2]{#bf{#it{ATLAS}} Internal}}{#sqrt{s}=13 TeV, 140.1 fb^{-1}}}"

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


f = TFile("/data/at3/scratch2/cbuxovaz/VLQAnalysisRun2/PairProd_SignalMC_21.2.121-htztx-3_1lepton_SR_and_CR_kin_February_03_2022/mc16a/outVLQAna_ttbarlight_nominal_HIST.root")

varList = {"meff", "jets_n", "MVAScore", "met"}

rebinDict = {"RCMHiggs_jets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             "RCMTop_jets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             #"meff" : "0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,4100,4200,4300,4400,4500,4600,4700,4800,4900,5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,6000,6100,6200,6300,6400,6500,6600,6700,6800,6900,7000",
             #"meff" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,5000",
             "meff" :"0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,4000,5000,6000,7000",
             #"meffred" : "0,100,400,500,600,700,800,900,1000,1200,1400,1600,2000,5000",
             #"meffred" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500,5000",
             "met" : "0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700",
             "hthad" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "htall" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "htred" : "0,100,200,300,400,500,600,700,800,900,1000,1200,1400,1600,2000,2500,3000,3500",
             "jets_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "jet0_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "trkbjets_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "trkbjet0_pt" : "0,20,40,60,80,100,120,160,200,240,280,320,360,400,500,600,700,800,1000",
             "RCjets_m" : "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,180,190,200,240,280,320,400,500",
             "jets_n" : "-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,15.5"}
             #"jets_n" : "-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,15.5"}

regList={"c1lep5jin2bex0Hex1VTex0_1Lwin", "c1lep5jin3bex0Hex1VTex0_1Lwin", "c1lep5jin4bin0Hex1VTex0_1Lwin", 
         "c1lep5jin2bex0Hex1VTex0Lex", "c1lep5jin2bex0Hex1VTex1Lex",
         "c1lep5jin3bex2Min3Jin0HexHighMVAScore", "c1lep5jin3bex2Min3Jin0HexMidMVAScore", "c1lep5jin3bex2Min3Jin0HexLowMVAScore",
         "c1lep5jin4bin2Min3Jin0HexHighMVAScore", "c1lep5jin4bin2Min3Jin0HexMidMVAScore", "c1lep5jin4bin2Min3Jin0HexLowMVAScore",
         "c1lep5jin3bex2Min3Jin1HinHighMVAScore", "c1lep5jin3bex2Min3Jin1HinMidMVAScore", "c1lep5jin3bex2Min3Jin1HinLowMVAScore",
         "c1lep5jin4bin2Min3Jin1HinHighMVAScore", "c1lep5jin4bin2Min3Jin1HinMidMVAScore", "c1lep5jin4bin2Min3Jin1HinLowMVAScore",
         "c1lep5jex3bex2Min3JinHighMVAScore", "c1lep5jex4bin2Min3JinHighMVAScore",
         "c1lep6jin3bex2Min3Jin0HexHighMVAScore", "c1lep6jin3bex2Min3Jin0HexMidMVAScore", "c1lep6jin3bex2Min3Jin0HexLowMVAScore",
         "c1lep6jin4bin2Min3Jin0HexHighMVAScore", "c1lep6jin4bin2Min3Jin0HexMidMVAScore", "c1lep6jin4bin2Min3Jin0HexLowMVAScore",
         "c1lep6jin3bex2Min3Jin1HinHighMVAScore", "c1lep6jin3bex2Min3Jin1HinMidMVAScore", "c1lep6jin3bex2Min3Jin1HinLowMVAScore",
         "c1lep6jin4bin2Min3Jin1HinHighMVAScore", "c1lep6jin4bin2Min3Jin1HinMidMVAScore", "c1lep6jin4bin2Min3Jin1HinLowMVAScore"}

varConfig = open("variableConfig.txt", "w+")

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
            varConfig.write("RESMIN : 0.\n")
            varConfig.write("RESMAX : 1.5\n")
            varConfig.write("YMIN   : 0.\n")
            if(var == "meff"):
                varConfig.write("XMIN   : 1000\n")
                varConfig.write("XMAX   : 7000\n")
            #varConfig.write("ISLOGY : TRUE\n")
            varConfig.write("DOWIDTH : TRUE\n")
            if(var in rebinDict):
                varConfig.write("REBINVAR : "+rebinDict[var]+"\n")
            
            if(var == "leadingdR_RCMHiggsRCMHiggs" or var == "leadingdR_RCMHiggsRCMV" or var == "leadingdR_RCMHiggsRCMTop" or var == "leadingdR_RCMVRCMV"
               or var == "leadingdR_RCMVRCMTop" or var == "leadingdR_RCMTopRCMTop"):
                varConfig.write("XMIN   : 0.\n")

            if(var == "dRmin_RCMHiggsRCMHiggs" or var == "dRmin_RCMHiggsRCMV" or var == "dRmin_RCMHiggsRCMTop" or var == "dRmin_RCMVRCMV"
               or var == "dRmin_RCMVRCMTop" or var == "dRmin_RCMTopRCMTop"):
                varConfig.write("XMAX   : 4.5\n")

            if(var == "recoHtHt_minMAsymm" or var == "recoHtZt_minMAsymm" or var == "recoHtWb_minMAsymm"):
                varConfig.write("XMIN   : 0.\n")
                varConfig.write("XMAX   : 1000.\n")

            #pVLQ_regions
            varConfig.write(GenerateRegionLabel(reg))
            varConfig.write("\n")

        else:
            print("WARNING! Variable "+fullName+" not found in input file!")

f.Close()
varConfig.write("END")
varConfig.close()
