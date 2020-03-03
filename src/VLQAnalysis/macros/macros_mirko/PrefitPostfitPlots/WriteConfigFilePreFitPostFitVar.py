import os, sys
import BinningPreFitPostFitVar as filebins


histopath= "/nfs/at3/scratch2/mcasolino/NEWTRexFitter/TtHFitter/config/"

 
varlist=[
    "met_zoom",
    "lep0_pt_zoom",
    "hthad_zoom",
    "jets_n",
    "bjets_n",
    "mbb_mindR",
    "RCjets_pt",
    "RCjets_m",
    "RCjet0_pt",
    "RCjet0_m",
    "RCTTMass_jets_n",
]

prettyvarlist=[
    "E_{T}^{miss} [GeV]",
    "Lepton p_{T} [GeV]",
    "H_{T}^{had} [GeV]",
    "Jet multiplicity",
    "b-jet multiplicity",
    "m_{bb}^{min#DeltaR} [GeV]",
    "Leading RC jet mass [GeV]",
    "Leading RC jet pt [GeV]",
    "RC jet mass [GeV]",
    "RC jet pt [GeV]",
    "Mass-tagged jet multiplicity",
    
    
]






print "START"

for idxs,s in enumerate(varlist):
    inputfile=open('configFile_Unblind_UEDRPP1400_PREPOST_MASTER.txt','r')
    output = open('configFile_UEDRPP1400_PREPOST_'+s+'.txt','w')
    
    bins=[]
    width=[]
    print s
    if "met" in s:
        bins=filebins.metbinning
        width=filebins.metbinwidth
    
    if "lep" in s:
        bins=filebins.lepptbinning
        width=filebins.lepptbinwidth
    
    if "ht" in s:
        bins=filebins.htbinning
        width=filebins.htbinwidth        

    if "jet" in s:
        width=filebins.jetbinwidth
        bins=filebins.jetbinning
        if "bjet" in s:
            bins=filebins.bjetbinning
        if "RCTTMass_jets_n" in s:
            bins=filebins.hotjetbinning

        
    if "mbb" in s:
        inputfile=open('configFile_Unblind_UEDRPP1400_PREPOST_MASTER_mbb.txt','r')
        bins=filebins.mbbbinning
        width=filebins.mbbbinwidth

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

        if "mbb" in s:
            if 'BINNINGPreselection' in line:
                line=line.replace("BINNINGPreselection",bins[0])
            if 'BINNING0H6j2b' in line:
                line=line.replace("BINNING0H6j2b",bins[0])
            if 'BINNING1H6j2b' in line:
                line=line.replace("BINNING1H6j2b",bins[0])
            if 'BINNING2H6j2b' in line:
                line=line.replace("BINNING2H6j2b",bins[0])
            if 'BINNING0H6j3b' in line:
                line=line.replace("BINNING0H6j3b",bins[0])
            if 'BINNING0H6j4b' in line:
                line=line.replace("BINNING0H6j4b",bins[1])
            if 'BINNING1H6j3b' in line:
                line=line.replace("BINNING1H6j3b",bins[2])
            if 'BINNING1H6j4b' in line:
                line=line.replace("BINNING1H6j4b",bins[3])

            if 'WIDTHPreselection' in line:
                line=line.replace("WIDTHPreselection",width[0])
            if 'WIDTH0H6j2b' in line:
                line=line.replace("WIDTH0H6j2b",width[1])
            if 'WIDTH1H6j2b' in line:
                line=line.replace("WIDTH1H6j2b",width[2])
            if 'WIDTH2H6j2b' in line:
                line=line.replace("WIDTH2H6j2b",width[3])
            if 'WIDTH0H6j3b' in line:
                line=line.replace("WIDTH0H6j3b",width[4])
            if 'WIDTH0H6j4b' in line:
                line=line.replace("WIDTH0H6j4b",width[5])
            if 'WIDTH1H6j3b' in line:
                line=line.replace("WIDTH1H6j3b",width[6])
            if 'WIDTH1H6j4b' in line:
                line=line.replace("WIDTH1H6j4b",width[7])

        else:
            
            #print s

            if 'BINNINGPreselection' in line:
                line=line.replace("BINNINGPreselection",bins[0])
            if 'BINNING0H5j2b' in line:
                line=line.replace("BINNING0H5j2b",bins[1])
            if 'BINNING0H5j3b' in line:
                line=line.replace("BINNING0H5j3b",bins[2])
            if 'BINNING0H5j4b' in line:
                line=line.replace("BINNING0H5j4b",bins[3])
            if 'BINNING1H5j2b' in line:
                line=line.replace("BINNING1H5j2b",bins[4])
            if 'BINNING1H5j3b' in line:
                line=line.replace("BINNING1H5j3b",bins[5])
            if 'BINNING1H5j4b' in line:
                line=line.replace("BINNING1H5j4b",bins[6])
            if 'BINNING2H5j2b' in line:
                line=line.replace("BINNING2H5j2b",bins[7])
            if 'BINNING2H5j3b' in line:
                line=line.replace("BINNING2H5j3b",bins[8])
            if 'BINNING2H5j4b' in line:
                line=line.replace("BINNING2H5j4b",bins[9])
            if 'BINNING0H6j2b' in line:
                line=line.replace("BINNING0H6j2b",bins[10])
            if 'BINNING0H6j3b' in line:
                line=line.replace("BINNING0H6j3b",bins[11])
            if 'BINNING0H6j4b' in line:
                line=line.replace("BINNING0H6j4b",bins[12])
            if 'BINNING1H6j2b' in line:
                line=line.replace("BINNING1H6j2b",bins[13])
            if 'BINNING1H6j3bLowMbb' in line:
                line=line.replace("BINNING1H6j3bLowMbb",bins[14])
            if 'BINNING1H6j4bLowMbb' in line:
                line=line.replace("BINNING1H6j4bLowMbb",bins[15])
            if 'BINNING1H6j3bHighMbb' in line:
                line=line.replace("BINNING1H6j3bHighMbb",bins[16])
            if 'BINNING1H6j4bHighMbb' in line:
                line=line.replace("BINNING1H6j4bHighMbb",bins[17])
            if 'BINNING2H6j2b' in line:
                line=line.replace("BINNING2H6j2b",bins[18])
            if 'BINNING2H6j3b' in line:
                line=line.replace("BINNING2H6j3b",bins[19])
            if 'BINNING2H6j4b' in line:
                line=line.replace("BINNING2H6j4b",bins[20])
        
            if 'WIDTHPreselection' in line:
                line=line.replace("WIDTHPreselection",width[0])
            if 'WIDTH0H5j2b' in line:
                line=line.replace("WIDTH0H5j2b",width[1])
            if 'WIDTH0H5j3b' in line:
                line=line.replace("WIDTH0H5j3b",width[2])
            if 'WIDTH0H5j4b' in line:
                line=line.replace("WIDTH0H5j4b",width[3])
            if 'WIDTH1H5j2b' in line:
                line=line.replace("WIDTH1H5j2b",width[4])
            if 'WIDTH1H5j3b' in line:
                line=line.replace("WIDTH1H5j3b",width[5])
            if 'WIDTH1H5j4b' in line:
                line=line.replace("WIDTH1H5j4b",width[6])
            if 'WIDTH2H5j2b' in line:
                line=line.replace("WIDTH2H5j2b",width[7])
            if 'WIDTH2H5j3b' in line:
                line=line.replace("WIDTH2H5j3b",width[8])
            if 'WIDTH2H5j4b' in line:
                line=line.replace("WIDTH2H5j4b",width[9])
            if 'WIDTH0H6j2b' in line:
                line=line.replace("WIDTH0H6j2b",width[10])
            if 'WIDTH0H6j3b' in line:
                line=line.replace("WIDTH0H6j3b",width[11])
            if 'WIDTH0H6j4b' in line:
                line=line.replace("WIDTH0H6j4b",width[12])
            if 'WIDTH1H6j2b' in line:
                line=line.replace("WIDTH1H6j2b",width[13])
            if 'WIDTH1H6j3bLowMbb' in line:
                line=line.replace("WIDTH1H6j3bLowMbb",width[14])
            if 'WIDTH1H6j4bLowMbb' in line:
                line=line.replace("WIDTH1H6j4bLowMbb",width[15])
            if 'WIDTH1H6j3bHighMbb' in line:
                line=line.replace("WIDTH1H6j3bHighMbb",width[16])
            if 'WIDTH1H6j4bHighMbb' in line:
                line=line.replace("WIDTH1H6j4bHighMbb",width[17])
            if 'WIDTH2H6j2b' in line:
                line=line.replace("WIDTH2H6j2b",width[18])
            if 'WIDTH2H6j3b' in line:
                line=line.replace("WIDTH2H6j3b",width[19])
            if 'WIDTH2H6j4b' in line:
                line=line.replace("WIDTH2H6j4b",width[20])
        


                



        
        #print line
        
        output.write(line)
        
            
    output.close()

inputfile.close()
