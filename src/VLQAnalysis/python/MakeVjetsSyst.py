import ROOT as root
import time
import sys
from array import *
import math
import os
import numpy

import argparse
#from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

###.......................................................
def main(args):

   ##____________________________________________________
   ## Options
   parser = argparse.ArgumentParser()
   parser.add_argument("--inputDir",dest="inputDir",
                     help="repository for the TRex files are located",
                     action="store",default="")
   parser.add_argument("--outputDir",dest="outputDir",
                     help="repository where to put the modified files",
                     action="store",default="NewVjetsFile/")
   parser.add_argument("--doLepton",dest="doLepton",
                     help="consider 1L regions",
                     action="store_true",default=False)
   parser.add_argument("--doZeroLepton",dest="doZeroLepton",
                     help="consider 0L regions",
                     action="store_true",default=False)
   parser.add_argument("--allRegions",dest="allRegions",
                     help="Use all regions",
                     action="store_true",default=False)
   parser.add_argument("--doSR",dest="doSR",
                     help="Use signal regions",
                     type=int,action="store",default=1)
   parser.add_argument("--doVR",dest="doVR",
                     help="Use validation regions",
                     type=int,action="store",default=0)
   parser.add_argument("--doPresel",dest="doPresel",
                     help="Use preselection regions",
                     type=int,action="store",default=0)
   parser.add_argument("--otherVariables",dest="otherVariables",
                     help="Do variables other than meff and MVAScore",
                     action="store_true",default=False)
   parser.add_argument("--sample",dest="sample",
                     help="Name of sample to process",
                     action="store",default="Wjets")
   parser.add_argument("--moduleKeys",dest="moduleKeys",
                     help="Comma separated list of keys of region dictionary modules",
                     action="store",default="MVA")

   args = parser.parse_args(args)

   outputDir=args.outputDir
   inputDir=args.inputDir
   doLepton=args.doLepton
   doZeroLepton=args.doZeroLepton
   allRegions=args.allRegions
   doSR=args.doSR
   doVR=args.doVR
   doPresel=args.doPresel
   otherVariables=args.otherVariables
   sample=args.sample
   moduleKeys=args.moduleKeys.split(",")

   root.gROOT.SetBatch(1)
   root.gStyle.SetOptTitle(0)
   root.gStyle.SetPalette(1)
   root.TH1.SetDefaultSumw2(1)
   root.gStyle.SetOptStat(0)
   
   varlist=['meff', 'MVAScore']

   if otherVariables:

      if(doZeroLepton):
         varlist +=  ['mtbmin', 'met',
                      'jets_n', 'bjets_n', 'RCMTT_jets_n', 
                      'RCMHiggs_jets_n', 'RCMTop_jets_n', 'RCMV_jets_n',
                      'mvlq0_RCTTM_drmax', 'mvlq0_RCTTM_drmax', 'mvlq1_rcj_drmax', 'mvlq1_rcj_drmax',
                      'RCjet0_pt', 'RCjet1_pt', 'RCjet2_pt', 'RCMTop0_pt', 'RCMHiggs0_pt', 'RCMV0_pt',
                      'RCMV0_nconsts', 'RCMV0_nbconsts', 'RCMHiggs0_nconsts', 'RCMHiggs0_nbconsts',
                      'dRmin_RCjets', 'dRmin_RCMTT', 'dPhiavg_RCjets', 'dEtaavg_RCjets',
                      'dEtamin_RCjets', 'dEtamin_RCMTT', 
                      'leadingdR_RCjets', 'leadingdPhi_RCjets', 'dPhiavg_RCMTTMET']

      if(doLepton):
         varlist +=  ['mtbmin', 'met', 
                      'ptw', 'residualMET',
                      'RCMHiggs_jets_n', 'RCMTop_jets_n', 'RCMV_jets_n',
                      'RCjets_n', 'jets_n', 'bjets_n',
                      'RCjet0_pt', 'leptop_pt', 'dPhimin_RCMTT',
                      'leadingdEta_RCMTT', 'leadingdEta_RCjets', 'leadingdPhi_RCjets',
                      'mvlq0_RCTTM_drmax', 'mvlq0_RCTTM_drmax', 'mvlq1_rcj_drmax', 'mvlq1_rcj_drmax',
                      'RCMV0_pt', 'RCMV0_eta',
                      'RCMHiggs0_pt', 'RCMHiggs0_eta',
                      'RCMTop0_pt', 'RCMTop0_eta']
         
   ##________________________________________________________
   ## Getting list of region modules
   module_list = []

   if "MVA" in moduleKeys:
      import regions_dictionary_pVLQ_newAna_MVA_regions as pVLQ_newAna_MVA_regions
      module_list += [pVLQ_newAna_MVA_regions]
   if "BOT" in moduleKeys:
      import regions_dictionary_pVLQ_newAna_boosted_object_cut_regions as pVLQ_newAna_BOT_regions
      module_list += [pVLQ_newAna_BOT_regions]
   if "OLD" in moduleKeys:
      import regions_dictionary_pVLQ as pVLQ_oldAna_BOT_regions
      module_list += [pVLQ_oldAna_BOT_regions]
   ##.........................................................
   
   ##________________________________________________________
   ## Defining the list of regions to look at
   Regions = []
   if allRegions:
      Regions = [{'name':"all"}]
   else:
      if( doLepton ):
         for module in module_list:
            if( doSR ):
               Regions += module.fit_regions_1l
            if( doVR ):
               Regions += module.validation_regions_1l
            if( doPresel ):
               Regions += module.preselection_regions_1l
      if( doZeroLepton ):            
         for module in module_list:
            if( doSR ):
               Regions += module.fit_regions_0l
            if( doVR ):
               Regions += module.validation_regions_0l
            if( doPresel ):
               Regions += module.preselection_regions_0l

   ##________________________________________________________
   ## Small utility for histograms list
   def GetKeyNames( iF, dir = "" ):
      iF.cd(dir)
      return [key.GetName() for key in root.gDirectory.GetListOfKeys()]
   ##........................................................

   ##........................................................
   start =time.time()
   print( "START")
   if(os.path.isdir(outputDir)):
      print( "Directory already exists")
   else:
      os.system("mkdir -p "+outputDir)

   ##________________________________________________________
   ## List of weight variations

   weight_list=["weight_pmg_muR05__muF05",
                "weight_pmg_muR05__muF10",
                "weight_pmg_muR10__muF05",
                "weight_pmg_muR10__muF20",
                "weight_pmg_muR20__muF10",
                "weight_pmg_muR20__muF20"]

   ew_weight_list=["weight_pmg_muR10__muF10_assew",
                "weight_pmg_muR10__muF10_expassew",
                "weight_pmg_muR10__muF10_multiassew"]

   os.system("cp "+inputDir+"/"+sample+".root "+outputDir+"/"+sample+".root ")
   sampleFile=root.TFile.Open(outputDir+"/"+sample+".root", "UPDATE")

   if len(Regions)==1 and Regions[0]['name'].upper()=="ALL":
      keyList = GetKeyNames(ttbbfile)
      Regions = []
      for key in keyList:
         if key.find("meff")==-1: continue
         if key.find("meff_")>-1: continue
         temp_region = key.replace("_meff","")
         Regions += [{'name':temp_region}]

   for r,region in enumerate(Regions): 

      reg = region['name'].replace("HTX_","")
      binning=region['binning']
      xbins = numpy.array(binning.split(','),float)
      xbins=numpy.insert(xbins,0,0.)

      for v,var in enumerate(varlist):

         nominalhisto=sampleFile.Get(reg+"_"+var)
         #print reg+"_"+var
         nominalhisto.SetDirectory(0)
         if var == 'meff':
            nominalhisto=nominalhisto.Rebin(len(xbins)-1,"",xbins)
         
         uphisto=nominalhisto.Clone()
         uphisto.SetDirectory(0)
         uphisto.SetName(reg+"_"+var+"_weight_muRF_up")
         
         downhisto=nominalhisto.Clone()
         downhisto.SetDirectory(0)
         downhisto.SetName(reg+"_"+var+"_weight_muRF_down")

         ew_uphisto=nominalhisto.Clone()
         ew_uphisto.SetDirectory(0)
         ew_uphisto.SetName(reg+"_"+var+"_weight_EW_up")

         ew_downhisto=nominalhisto.Clone()
         ew_downhisto.SetDirectory(0)
         ew_downhisto.SetName(reg+"_"+var+"_weight_EW_down")

         for weight_var in weight_list:
            #print reg+"_"+var+"_"+weight_var
            weightvarhisto=sampleFile.Get(reg+"_"+var+"_"+weight_var)
            weightvarhisto.SetDirectory(0)
            if var == 'meff':
               weightvarhisto=weightvarhisto.Rebin(len(xbins)-1,"",xbins)
            sampleFile.cd()
            weightvarhisto.Write()

            for i in range(1,nominalhisto.GetNbinsX()+1):
               bc_w = weightvarhisto.GetBinContent(i)
               if( bc_w < downhisto.GetBinContent(i) ):
                  downhisto.SetBinContent(i,bc_w)
               if( bc_w > uphisto.GetBinContent(i) ):
                  uphisto.SetBinContent(i,bc_w)

         for ew_weight_var in ew_weight_list:
            ew_weightvarhisto=sampleFile.Get(reg+"_"+var+"_"+ew_weight_var)
            ew_weightvarhisto.SetDirectory(0)
            if var == 'meff':
               ew_weightvarhisto=ew_weightvarhisto.Rebin(len(xbins)-1,"",xbins)
            sampleFile.cd()
            ew_weightvarhisto.Write()

            for i in range(1,nominalhisto.GetNbinsX()+1):
               bc_w = ew_weightvarhisto.GetBinContent(i)
               if( bc_w < ew_downhisto.GetBinContent(i) ):
                  ew_downhisto.SetBinContent(i,bc_w)
               if( bc_w > ew_uphisto.GetBinContent(i) ):
                  ew_uphisto.SetBinContent(i,bc_w)


            sampleFile.cd()
            uphisto.Write()
            downhisto.Write()
            ew_uphisto.Write()
            ew_downhisto.Write()
            nominalhisto.Write()

   sampleFile.Close()
        
   elapsed=time.time()-start
   elapsed=elapsed/60
   print( elapsed, " minuti trascorsi")
   print( "END")

###.......................................................


###.......................................................
if __name__ == '__main__':
   main(sys.argv[1:])
