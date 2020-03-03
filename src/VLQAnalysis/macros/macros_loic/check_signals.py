#!/bin/python

from ROOT import *
import glob


##________________________________________________________
## Small utility for histograms list
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in gDirectory.GetListOfKeys()]
##........................................................

old_path = "/nfs/pic.es/user/l/lvalery/scratch2/VLQFitOutputs/2017_05_01_VLQAnalysis-00-03-09-03_WithNNLO_MetSigCut/InputFiles/1lep/"
new_path = "/nfs/pic.es/user/l/lvalery/scratch2/VLQFitOutputs/2017_05_15_VLQAnalysis-00-03-09-06/InputFiles/1lep/"
list_old_files = glob.glob(old_path+"/VLQ_TT_1000_TDoublet*")

hist_list_old = []
hist_list = []
for f in list_old_files:
    oldF = TFile(f,"READ")
    newF = TFile(f.replace(old_path,new_path),"READ")
    hist_list_old = GetKeyNames(oldF)
    hist_list = GetKeyNames(newF)
    for hist in hist_list:
        if not hist in hist_list_old:
            continue
        oldH = oldF.Get(hist)
        newH = newF.Get(hist)
        oldI = oldH.Integral()
        newI = newH.Integral()
        if(oldI==0):
            continue
        relative_diff = (newI-oldI)/oldI
        if(abs(relative_diff)>0.10):
            com = "==> "
            com += hist
            com += "   "
            com += " Difference: " + `relative_diff`
            print(com)
