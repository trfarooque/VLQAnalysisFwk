import os, sys
import BinningPreFitPostFitVar_0lep as filebins


histopath= "/nfs/at3/scratch2/mcasolino/NEWTRexFitter/TtHFitter/config/"

 
varlist=[
    "met_zoom",
    "jets_n",
    "bjets_n",
    "mtbmin_zoom",
    "RCjets_pt",
    "RCjets_m",
    "RCjet0_pt",
    "RCjet0_m",
    "RCTTMass_jets_n",
]

prettyvarlist=[
    "E_{T}^{miss} [GeV]",
    "Jet multiplicity",
    "b-jet multiplicity",
    "m_{T}^{bmin} [GeV]",
    "Leading RC jet mass [GeV]",
    "Leading RC jet pt [GeV]",
    "RC jet mass [GeV]",
    "RC jet pt [GeV]",
    "Mass-tagged jet multiplicity",
    
    
]






print "START"

for idxs,s in enumerate(varlist):
#    print s
    inputfile=open('configFile_Unblind_UEDRPP1400_PREPOST_MASTER_0LEP.txt','r')
    output = open('configFile_UEDRPP1400_PREPOST_0LEP_'+s+'.txt','w')
    
    bins=[]
    width=[]
    
    if "met" in s:
        bins=filebins.metbinning
        width=filebins.metbinwidth
    
    if "jet" in s:
        width=filebins.jetbinwidth
        bins=filebins.jetbinning
        if "bjet" in s:
            bins=filebins.bjetbinning
        if "RCTTMass_jets_n" in s:
            bins=filebins.hotjetbinning
        

    if "mtbmin" in s:
        bins=filebins.mtbminbinning
        width=filebins.mtbminbinwidth

    if "RC" in s:
	if "pt" in s:
            bins=filebins.fjptbinning
            width=filebins.fjptbinwidth
	if "_m" in s:
            bins=filebins.fjmbinning
            width=filebins.fjmbinwidth
            
    for line in inputfile:
        if "PREPOST_VARNAME" in line:
            line=line.replace("VARNAME",s)
        if 'xxxxx' in line:
            line=line.replace("xxxxx",s)
        if 'yyyyy' in line:
            line=line.replace("yyyyy",prettyvarlist[idxs])

        
        if 'BINNINGPreselection' in line:
            line=line.replace("BINNINGPreselection",bins[0])
        if 'BINNING0H6j2b' in line:
            line=line.replace("BINNING0H6j2b",bins[1])
        if 'BINNING0H6j3b' in line:
            line=line.replace("BINNING0H6j3b",bins[2])
        if 'BINNING0H6j4b' in line:
            line=line.replace("BINNING0H6j4b",bins[3])
        if 'BINNING1H6j2b' in line:
            line=line.replace("BINNING1H6j2b",bins[4])
        if 'BINNING1H6j3b' in line:
            line=line.replace("BINNING1H6j3b",bins[5])
        if 'BINNING1H6j4b' in line:
            line=line.replace("BINNING1H6j4b",bins[6])
        if 'BINNING2H6j2b' in line:
            line=line.replace("BINNING2H6j2b",bins[7])
        if 'BINNING2H6j3b' in line:
            line=line.replace("BINNING2H6j3b",bins[8])
        if 'BINNING2H6j4b' in line:
            line=line.replace("BINNING2H6j4b",bins[9])
        if 'BINNING0H7j2b' in line:
            line=line.replace("BINNING0H7j2b",bins[10])
        if 'BINNING0H7j3b' in line:
            line=line.replace("BINNING0H7j3b",bins[11])
        if 'BINNING0H7j4b' in line:
            line=line.replace("BINNING0H7j4b",bins[12])
        if 'BINNING1H7j2b' in line:
            line=line.replace("BINNING1H7j2b",bins[13])
        if 'BINNING1H7j3bLowMtbmin' in line:
            line=line.replace("BINNING1H7j3bLowMtbmin",bins[14])
        if 'BINNING1H7j4bLowMtbmin' in line:
            line=line.replace("BINNING1H7j4bLowMtbmin",bins[15])
        if 'BINNING1H7j3bHighMtbmin' in line:
            line=line.replace("BINNING1H7j3bHighMtbmin",bins[16])
        if 'BINNING1H7j4bHighMtbmin' in line:
            line=line.replace("BINNING1H7j4bHighMtbmin",bins[17])
        if 'BINNING2H7j2b' in line:
            line=line.replace("BINNING2H7j2b",bins[18])
        if 'BINNING2H7j3bLowMtbmin' in line:
            line=line.replace("BINNING2H7j3bLowMtbmin",bins[19])
        if 'BINNING2H7j3bHighMtbmin' in line:
            line=line.replace("BINNING2H7j3bHighMtbmin",bins[20])
        if 'BINNING2H7j4b' in line:
            line=line.replace("BINNING2H7j4b",bins[21])
        
        if 'WIDTHPreselection' in line:
            line=line.replace("WIDTHPreselection",width[0])
        if 'WIDTH0H6j2b' in line:
            line=line.replace("WIDTH0H6j2b",width[1])
        if 'WIDTH0H6j3b' in line:
            line=line.replace("WIDTH0H6j3b",width[2])
        if 'WIDTH0H6j4b' in line:
            line=line.replace("WIDTH0H6j4b",width[3])
        if 'WIDTH1H6j2b' in line:
            line=line.replace("WIDTH1H6j2b",width[4])
        if 'WIDTH1H6j3b' in line:
            line=line.replace("WIDTH1H6j3b",width[5])
        if 'WIDTH1H6j4b' in line:
            line=line.replace("WIDTH1H6j4b",width[6])
        if 'WIDTH2H6j2b' in line:
            line=line.replace("WIDTH2H6j2b",width[7])
        if 'WIDTH2H6j3b' in line:
            line=line.replace("WIDTH2H6j3b",width[8])
        if 'WIDTH2H6j4b' in line:
            line=line.replace("WIDTH2H6j4b",width[9])
        if 'WIDTH0H7j2b' in line:
            line=line.replace("WIDTH0H7j2b",width[10])
        if 'WIDTH0H7j3b' in line:
            line=line.replace("WIDTH0H7j3b",width[11])
        if 'WIDTH0H7j4b' in line:
            line=line.replace("WIDTH0H7j4b",width[12])
        if 'WIDTH1H7j2b' in line:
            line=line.replace("WIDTH1H7j2b",width[13])
        if 'WIDTH1H7j3bLowMtbmin' in line:
            line=line.replace("WIDTH1H7j3bLowMtbmin",width[14])
        if 'WIDTH1H7j4bLowMtbmin' in line:
            line=line.replace("WIDTH1H7j4bLowMtbmin",width[15])
        if 'WIDTH1H7j3bHighMtbmin' in line:
            line=line.replace("WIDTH1H7j3bHighMtbmin",width[16])
        if 'WIDTH1H7j4bHighMtbmin' in line:
            line=line.replace("WIDTH1H7j4bHighMtbmin",width[17])
        if 'WIDTH2H7j2b' in line:
            line=line.replace("WIDTH2H7j2b",width[18])
        if 'WIDTH2H7j3bLowMtbmin' in line:
            line=line.replace("WIDTH2H7j3bLowMtbmin",width[19])
        if 'WIDTH2H7j3bHighMtbmin' in line:
            line=line.replace("WIDTH2H7j3bHighMtbmin",width[20])
        if 'WIDTH2H7j4b' in line:
            line=line.replace("WIDTH2H7j4b",width[21])
        


                



        
        #print line
        
        output.write(line)
        
            
    output.close()

inputfile.close()
