import ROOT as root
import time
import sys
from array import *
import math
import os
import numpy

from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("--inputDir",dest="inputDir",help="repository for the TRex files are located",action="store",default="")
parser.add_option("--outputDir",dest="outputDir",help="repository where to put the modified files",action="store",default="Extrapolated/")
parser.add_option("--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=False)
parser.add_option("--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=False)
parser.add_option("--allRegions",dest="allRegions",help="Use all regions",action="store_true",default=False)
parser.add_option("--doNominal",dest="doNominal",help="Read and extrapolate nominal histograms. ** This also changes extrapolation behaviour of sys histograms **",type=int,
                  action="store",default=0)
parser.add_option("--doWeightSys",dest="doWeightSys",help="Read and extrapolate weight systematics",type=int,
                  action="store",default=1)
parser.add_option("--doAltSys",dest="doAltSys",help="Read and extrapolate systematics from alternative samples",type=int,
                  action="store",default=0)
parser.add_option("--doSR",dest="doSR",help="Use signal regions",type=int,action="store",default=1)
parser.add_option("--doVR",dest="doVR",help="Use validation regions",type=int,action="store",default=0)
parser.add_option("--doPresel",dest="doPresel",help="Use preselection regions",type=int,action="store",default=0)
parser.add_option("--otherVariables",dest="otherVariables",help="Do variables other than meff",action="store_true",default=False)
parser.add_option("--sample",dest="sample",help="Name of sample to process",action="store",default="Singletop")
parser.add_option("--campaign",dest="campaign",help="MC campaign to process",action="store", default="")
parser.add_option("--moduleKeys",dest="moduleKeys",help="Comma separated list of keys of region dictionary modules",action="store",default="MVA")
parser.add_option("--debug",dest="debug",help="print debug messages",action="store_true",default=False)

(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
doLepton=options.doLepton
doZeroLepton=options.doZeroLepton
allRegions=options.allRegions
doNominal=options.doNominal
doWeightSys=options.doWeightSys
doAltSys=options.doAltSys
doSR=options.doSR
doVR=options.doVR
doPresel=options.doPresel
otherVariables=options.otherVariables
sample=options.sample
campaign=options.campaign
moduleKeys=options.moduleKeys.split(",")
debug=options.debug

if(campaign == ""):
   print "<!> ERROR:: No MC campaign specifiedg."
   sys.exit(-1)
   
## check option consistency ##
if doWeightSys and doAltSys:
   print "<!> ERROR:: doWeightSys=1 and doAltSys=1 are not allowed simultaneously."
   sys.exit(-1)

root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetPalette(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptStat(0)

varlist=["meff"]

if otherVariables:

   varlist +=["MVAScore"]

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

if debug:
   print 'Number of regions : ',len(Regions)

##________________________________________________________
## Small utility for histograms list
def GetKeyNames( iF, dir = "" ):
    iF.cd(dir)
    return [key.GetName() for key in root.gDirectory.GetListOfKeys()]
##........................................................


##__________________________________________________________
## Hardcoded extrapolation rules for regions
def GetExtrapolationRule(regname):

   extr_rule = {
      'doExtr':False, #extrapolate the uncertainty
      'reg_src':'',  #name of source region from which to extrapolate
      'src_list':[], #regions to sum in order to make source region
      'onlyShape':False, #only extrapolate shape while retaining normalisation of target region
      'xbins': None #binning of target region, to be read in from regions_dictionary
   }

   ##No extrapolation needed if region name does not contain MVAScore; 
   ##or if region is 3bex0Hex
   if ("MVAScore" not in regname) or ('5jin3bex2Min3Jin0Hex' in regname) :
      pass
   else:
      extr_rule['doExtr'] = True   

      if('4bin2Min3Jin0Hex' in regname):
         extr_rule['onlyShape'] = True
         extr_rule['src_list'] = [ regname.replace('4bin','3bex'), regname ]

      elif('LowMVAScore' in regname): ##These are 3bex1Hin and 4bin1Hin - merge the two
         extr_rule['src_list'] = ['c1lep5jin3bex2Min3Jin1HinLowMVAScore',
                                  'c1lep5jin4bin2Min3Jin1HinLowMVAScore']
      else: ## These are MMVA and HMVA regions - merge the two in each btag category 
         if('3bex' in regname):
            extr_rule['src_list'] = ['c1lep5jin3bex2Min3Jin1HinMidMVAScore',
                                     'c1lep5jin3bex2Min3Jin1HinHighMVAScore']
         else: #4bin
            extr_rule['src_list'] = ['c1lep5jin4bin2Min3Jin1HinMidMVAScore',
                                     'c1lep5jin4bin2Min3Jin1HinHighMVAScore']
   if debug:
      print "<INFO>: Region: ",regname#,'\n'
      print "doExtr: ",extr_rule['doExtr']#,'\n'
      print "onlyShape: ",extr_rule['onlyShape']#,'\n'
      print "src_list : "
      for src_reg in extr_rule['src_list']: 
         print src_reg+' , '
      print '\n'


   return extr_rule


##________________________________________________________________
## Get extrapolated histogram
def GetExtrapolatedHist(regname, varname, sysname, copySrc=True):

   h_ext = GetExtrapolatedHistCopy(regname, varname, sysname) if copySrc \
           else GetExtrapolatedHistVariation(regname, varname, sysname)

   return h_ext 


##________________________________________________________________
## In this version of the extrapolation, there is no multiplication
#  by the nominal histogram. Instead, the histograms from the source 
#  region are simply added together and copied to the target region. 
#  Target region normalisation can still be preserved if onlyShape is 
#  set to true.
def GetExtrapolatedHistCopy(regname, varname, sysname):

   varsysname = varname
   if sysname != "":
      varsysname = varname + "_" + sysname

   h_target = hist_dict[regname+'_'+varsysname]
   extr_rule = reg_rules[regname]
   xbins = extr_rule['xbins']
 
   if debug:
      print regname, " : ", xbins
   h_ext = h_target.Clone() #histogram to return

   # only extrapolate histograms in a region if asked by rule
   # only extrapolate nominal histograms if doNominal=True
   # othwerwise, simply copy the target histogram to the output file
   if( (doNominal or sysname!="") and extr_rule['doExtr'] ):

      h_ext.Reset()

      #Add the histograms from the source list
      for src_reg in extr_rule['src_list']:
         h_ext.Add( hist_dict[src_reg+'_'+varsysname] )

      #Restore target region normalisation if only extrapolating the shape 
      if extr_rule['onlyShape']:
         trgt_norm = h_target.Integral()
         src_norm = h_ext.Integral()
         norm_scale = trgt_norm/src_norm
         h_ext.Scale(norm_scale)

   h_ext=h_ext.Rebin(len(xbins)-1,"",xbins)

   return h_ext
##........................................................

##________________________________________________________
## Make the extrapolated systematic variation histogram 
#  from source and target region histograms
def GetExtrapolatedHistVariation(regname, varname, sysname):

   if(sysname==""):
      print "<!> ERROR: GetExtrapolatedHistVariation called on a nominal histogram"
      sys.exit(1)

   h_nom = hist_dict[regname+'_'+varname]
   h_sys = hist_dict[regname+'_'+varname+'_'+sysname]
   extr_rule = reg_rules[regname]
   xbins = extr_rule['xbins']
 
   h_sys_ext = None #histogram to return

   # only extrapolate histograms in a region if asked by rule
   # othwerwise, simply copy the target histogram to the output file
   if( extr_rule['doExtr'] ):

      h_src_nom = h_nom.Clone()
      h_src_nom.Reset()
      h_src_sys = h_nom.Clone()
      h_src_sys.Reset()

      #Add the histograms from the source list
      for src_reg in extr_rule['src_list']:
         h_src_nom.Add( hist_dict[src_reg+'_'+varname] )
         h_src_sys.Add( hist_dict[src_reg+'_'+varname+'_'+sysname] )

      h_sys_ext = h_nom.Clone(h_sys.GetName())
      #Rebin everything according to target region binning
      h_sys_ext=h_sys_ext.Rebin(len(xbins)-1,"",xbins)
      h_src_nom=h_src_nom.Rebin(len(xbins)-1,"",xbins)
      h_src_sys=h_src_sys.Rebin(len(xbins)-1,"",xbins)

      #Scale by the fractional uncertainties from the src region
      h_sys_ext.Multiply(h_src_sys)
      h_sys_ext.Divide(h_src_nom)

      #Restore target region normalisation uncertainty if only extrapolating the shape 
      if extr_rule['onlyShape']:
         trgt_sys_norm = h_sys.Integral()/h_nom.Integral()
         src_sys_norm = h_src_sys.Integral()/h_src_nom.Integral()
         norm_scale = trgt_sys_norm/src_sys_norm
         h_sys_ext.Scale(norm_scale)

   else:
      #Simply copy and rebin histogram if no extrapolation required
      h_sys_ext = h_sys
      h_sys_ext=h_sys_ext.Rebin(len(xbins)-1,"",xbins)

   return h_sys_ext
##........................................................


##........................................................
start =time.time()
print "START"
if(os.path.isdir(outputDir)):
    print "Directory already exists"
else:
    os.system("mkdir -p "+outputDir)


##________________________________________________________
## List of weight variations

weight_list=[]
if doWeightSys:
   weight_list += ["weight_pmg_muR10__muF20",
                "weight_pmg_muR10__muF05",
                "weight_pmg_muR20__muF10",
                "weight_pmg_muR05__muF10",
                "weight_pmg_Var3cUp",
                "weight_pmg_Var3cDown",
                "weight_pmg_isr_muRfac10__fsr_muRfac20",
                "weight_pmg_isr_muRfac10__fsr_muRfac05"]

alt_list=[""] #the nominal MC file
if doAltSys:
   alt_list += ["PowHer","aMCPy"]#,"DiagSub"]

#os.system("cp "+inputDir+"/"+sample+".root "+outputDir+"/"+sample+".root ") #WHY?

### -------------- Read from region dictionary -------------------
reg_rules = {} ##Extrapolation rules for each region

for region in Regions: 
   reg = region['name'].replace("HTX_","")
   reg_rules[reg] = GetExtrapolationRule(reg)

   binning=region['binning']
   xbins = numpy.array(binning.split(','),float)
   xbins=numpy.insert(xbins,0,0.)
   reg_rules[reg]['xbins'] = xbins


### -------------- Read all histograms -------------------
##Read all the histograms into a single dictionary
##This may not be an efficient way if there are too many regions and/or variables 
hist_dict = {}

for sys_suf in alt_list:

   if debug:
      print "sys: ",sys_suf,"."

   inputFile=root.TFile.Open(inputDir+"/"+sample+sys_suf+"."+campaign+".root", "READ")

   for region in Regions: 
   
      reg = region['name'].replace("HTX_","")

      for var in varlist:

         nominalhisto=inputFile.Get(reg+"_"+var)
         nominalhisto.SetDirectory(0)
         hist_dict[reg+"_"+var if sys_suf=="" else reg+"_"+var+"_"+sys_suf ] = nominalhisto

         if doWeightSys:
            for weight_var in weight_list:
               weightvarhisto=inputFile.Get(reg+"_"+var+"_"+weight_var)
               weightvarhisto.SetDirectory(0)
               hist_dict[reg+"_"+var+"_"+weight_var] = weightvarhisto

   if debug:
      print "Read all histo from sys: ",sys_suf,"."
   inputFile.Close()
### -------------- All histograms read -------------------

### -------------- Start extrapolation -------------------
for sys_suf in alt_list:

   outputFile=root.TFile.Open(outputDir+"/"+sample+sys_suf+"."+campaign+".root", "UPDATE")
   outputFile.cd()

   for regname,extr_rule in reg_rules.items(): 

      for varname in varlist:

         print regname, " : ", varname, " : ", sys_suf
         #copySrc = doNominal, because if the nominal histogram is also extrapolated from 
         #source regions then the sys variation can be taken from the source region directly
         h_alt_extr = GetExtrapolatedHist(regname, varname, sys_suf, copySrc=doNominal)
         h_alt_extr.Write()

         if doWeightSys:   
            for wgtsys in weight_list:
               h_wgt_extr = GetExtrapolatedHist(regname, varname, wgtsys, copySrc=doNominal)
               h_wgt_extr.Write()
      
   outputFile.Close()

### --------------  Extrapolation completed -------------------

elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
