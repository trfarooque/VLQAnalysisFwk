import ROOT
import argparse
import numpy as np
import scipy.optimize as spo
import scipy.integrate as spi
import re
import pandas as pd

## Useful common variables
campaigns = ["mc16a", "mc16d", "mc16e"]
nondijetbackgrounds = ["Dibosons", "Singletopschan", "Singletoptchan", "SingletopWtprod", "topEW", "ttbarbb", "ttbarcc", "ttbarlight", "Wjets", "Zjets"]
datafiles = ["Data.grp15.root","Data.grp16.root","Data.grp17.root","Data.grp18.root"]

def readFileList(filelist = "file_list.txt"):
    MCFactors = {}
    with open(filelist) as f:
        lines = f.readlines()
        for line in lines:
            if (re.search("BEGIN", line)):
                continue
            if (re.search("END", line)):
                return MCFactors
            splitline = line.split(" : ")
            if (re.search("KEY", splitline[0])):
                continue
            MCFactors[splitline[1]]=float(re.sub('\n','',splitline[2]))
    print("END not found in ", filelist)
    return MCFactors

def getData(MCdir, campaigns= ["mc16a", "mc16d", "mc16e"], hist="dPhi_jetmet", reg="c0lep6jin2bin"):
    
    dijetfiles = {"Dijet."+j : ROOT.TFile(MCdir+"/nominal/Dijet."+j+".root") for j in campaigns}
    BkgFiles = {i+"."+j : ROOT.TFile(MCdir+"/nominal/"+i+"."+j+".root") for i in nondijetbackgrounds for j in campaigns}
    DataFiles  = {i: ROOT.TFile(MCdir+"/nominal/"+i) for i in datafiles}
    
    HISTOGRAM=reg+"_"+hist

    MCFactors=readFileList()

    histosDijet={i: dijetfiles[i].Get(HISTOGRAM) for i in dijetfiles.keys()}
    histosBkg={i: BkgFiles[i].Get(HISTOGRAM) for i in BkgFiles.keys()}
    histosData={i: DataFiles[i].Get(HISTOGRAM) for i in DataFiles.keys()}
    
    nbinsX = histosDijet["Dijet.mc16a"].GetNbinsX()
    binCentres = [histosDijet["Dijet.mc16a"].GetBinCenter(i) for i in range (0, nbinsX+2)]
    #print(dir(histosDijet["Dijet.mc16a"]))
    
    dijetArrs = {j: np.array([histosDijet[j][i] for i in range (0, nbinsX+2)])*MCFactors[j+".root"] for j in histosDijet.keys()}
    BkgArrs = {j: np.array([histosBkg[j][i] for i in range(0, nbinsX+2)])*MCFactors[j+".root"] for j in histosBkg.keys()}
    DataArrs = {j: np.array([histosData[j][i] for i in range(0, nbinsX+2)]) for j in histosData.keys()}
    
    dijetErrArrs = {j: np.array([histosDijet[j].GetBinError(i) for i in range (0, nbinsX+2)])*MCFactors[j+".root"] for j in histosDijet.keys()}
    BkgErrArrs = {j: np.array([histosBkg[j].GetBinError(i) for i in range(0, nbinsX+2)])*MCFactors[j+".root"] for j in histosBkg.keys()}
    DataErrArrs = {j: np.array([histosData[j].GetBinError(i) for i in range(0, nbinsX+2)]) for j in histosData.keys()}
    
    # We know its symmetric
    # dijetErrArrsDown = {j: np.array([histosDijet[j].GetBinErrorLow(i) for i in range (0, nbinsX+2)])*MCFactors[j+".root"] for j in histosDijet.keys()}
    # BkgErrArrsDown = {j: np.array([histosBkg[j].GetBinErrorLow(i) for i in range(0, nbinsX+2)])*MCFactors[j+".root"] for j in histosBkg.keys()}
    # DataErrArrsDown = {j: np.array([histosData[j].GetBinErrorLow(i) for i in range(0, nbinsX+2)]) for j in histosData.keys()}
    
    # Merge together Bkg, data & dijet
    BkgVec = sum(BkgArrs.values())
    DjVec = sum(dijetArrs.values())
    DataVec = sum(DataArrs.values())
    
    BkgVecErr = sum(BkgErrArrs.values())
    DjVecErr = sum(dijetErrArrs.values())
    DataVecErr = sum(DataErrArrs.values())
    
    
    df = pd.DataFrame({"binCentre": binCentres, "diJet": DjVec, "diJetErr": DjVecErr, "Bkg": BkgVec, "BkgErr": BkgVecErr, "Data": DataVec, "DataErr": DataVecErr })
    df["Diff"] = df["Data"] - df["Bkg"]
    df["DiffErr"] = (df["DataErr"]**2 + df["BkgErr"]**2)**0.5
    return df
    
    
        
