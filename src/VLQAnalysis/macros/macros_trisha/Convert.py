#!/bin/python
import glob
import math
import os
import sys
#import pdgRounding

##_______________________________________________________________
##
def writePreamble( inFile, fout ):
    if catTogether and inFile.name.find("Syst_table")>-1:
        contents = inFile.readlines()
        contents.insert(0,"\\documentclass[a4paper,7pt]{article}\n")
        contents.insert(1,"\\usepackage[margin=1in]{geometry}\n")
        contents.insert(2,"\\paperheight = 1700pt\n")
        contents.insert(3,"\\paperwidth = 900pt\n")
        contents.insert(4,"\\usepackage{atbegshi}\n")
        contents.insert(5,"\\AtBeginDocument{\AtBeginShipoutNext{\AtBeginShipoutDiscard}}\n")
        contents.insert(6,"\\usepackage{float}\n")
        contents.insert(7,"\\restylefloat{table}\n")
        contents.insert(8,"\\begin{document}\n")
        contents.insert(9,"\\pagenumbering{gobble}\n")

        contents = "".join(contents)
        open(fout.name,"w").write(contents)
        return

    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True

    if separateTables and isSyst:
        fout.write("\\documentclass[a4paper,7pt]{article}\n")
        fout.write("\\usepackage[margin=1in]{geometry}\n")
        fout.write("\\paperheight = 1700pt\n")
        fout.write("\\paperwidth = 800pt\n")
        fout.write("\\begin{document}\n")
        fout.write("\\pagenumbering{gobble}\n")
    if not isSyst:
        fout.write("\\documentclass[a4paper,7pt]{article}\n")
        fout.write("\\usepackage[margin=1in]{geometry}\n")
        fout.write("\\begin{document}\n")
        fout.write("\\pagenumbering{gobble}\n")
        # fout.write("\\begin{table}\n")
        # fout.write("\\begin{center}\n")

##_______________________________________________________________
##
def writeEnd(inFile,fout):
    if catTogether and inFile.name.find("Syst_table")>-1:
        fout.write("\\end{document}\n")
        return

    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True
    if not isSyst:
        fout.write("\\end{document}\n")
        # fout.write("\\end{center}\n")
    if isSyst and separateTables:
        fout.write("\\end{table}\n")
        fout.write("\\end{document}\n")


##_______________________________________________________________
##
def convertAllLines( inFile, fout ):

    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True

    #gets all results properly formatted
    result = dumpVector(inFile)

    #nSplit
    n_split = 3
    if(isSyst): n_split = 1000

    #n_tables
    n_tables = 1

    # take out sVLQ signal from columns
    if isSyst and removeSignal:
        popsVLQ = False
        for r in result:
            if any('sVLQ' in s for s in r['columns']):
                popindex = r['columns'].index([i for i in r['columns'] if 'sVLQ' in i][0])
                popsVLQ = True
                break
        if popsVLQ:
            for r in result:
                r['columns'].pop(popindex)
    if not isSyst and removeSignal:
        popsVLQ = False
        for n,r in enumerate(result):
            if 'sVLQ' in r['sample']:
                popindex = n
                popsVLQ = True
                break
        if popsVLQ:
            result.pop(popindex)

    #gets the final table
    if len(result[0]['columns'])>n_split and (not isSyst):
        n_tables = int(math.ceil((len(result[0]['columns'])-1)/float(n_split)))
    elif isSyst:
        nSplit = len(result[0]['columns'])-1

    if verbose:
        print 'inFile.name ', inFile.name
        print 'n_split ', n_split
        print 'n_tables ', n_tables
        print 'isSyst ', isSyst

    fout.write("\\begin{table}[H]\n")

    for i_table in range(n_tables):
        fout.write("  \\centering\n")
        fout.write("  \\begin{tabular}{ | l | ")
        for i_col in range(0,min(n_split,len(result[0]['columns']) - 1 - n_split*i_table)):
            fout.write(" c | ")
        fout.write("}\n")

        fout.write("    \\hline \\hline \n")

        count_sample = 0
        for sample in result:
            line = "    " + sample['sample'] + " & "
            max_col = i_table*n_split+min(n_split+1,len(result[0]['columns']) - n_split*i_table)
            for i_col in range(i_table*n_split+1,max_col):
                line += sample['columns'][i_col]
                if i_col != max_col-1:
                    line += " & "
                else:
                    line += " \\\\ \n"

            fout.write(line)

            if count_sample==0 or (not isSyst and (count_sample==len(result)-3)):
                fout.write("    \\hline \n")
            count_sample+=1
        fout.write("    \\hline \\hline \n")

        fout.write("  \\end{tabular} \n")

        if labelEachTable and isSyst:
            fout.write("  \\caption{%s}\n"%getRegionLabel(inFile.name))

        fout.write("\\end{table}\n")
        fout.write("\n\n")

##_______________________________________________________________
##
def dumpVector(inFile):
    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True
    #Counts all the lines
    total_lines=0
    for inLine in inFile:
        if inLine.replace(" ","").replace("\n","")=="": continue
        total_lines += 1

    inFile.seek(0)
    line_n = 0

    result = []
    for inLine in inFile:

        #just for the splitting
        columns_splitted = []
        sample_name = ""

        if inLine.replace(" ","").replace("\n","")=="": continue
        temp_line = inLine


        #Change some known words
        temp_line = temp_line.replace("#splitline", "\\makecell")
        temp_line = temp_line.replace("}{", " \\\\ ")
        temp_line = temp_line.replace("t_{l}", "$t_{l}$")
        temp_line = temp_line.replace("t_{h}", "$t_{h}$")
        temp_line = temp_line.replace("t#bar{t}+light", "$t\\bar{t}+\geq 1l$")
        temp_line = temp_line.replace("t#bar{t}+cc", "$t\\bar{t}+\geq 1c$")
        temp_line = temp_line.replace("t#bar{t}+bb", "$t\\bar{t}+\geq 1b$")
        temp_line = temp_line.replace("t#bar{t}+c#bar{c}", "$t\\bar{t}+\geq 1c$")
        temp_line = temp_line.replace("t#bar{t}+b#bar{b}", "$t\\bar{t}+\geq 1b$")
        temp_line = temp_line.replace("t#bar{t}+#geq1c", "$t\\bar{t}+\geq 1c$")
        temp_line = temp_line.replace("t#bar{t}+#geq1b", "$t\\bar{t}+\geq 1b$")
        temp_line = temp_line.replace(" t#bar{t}", " $t\\bar{t}$")
        temp_line = temp_line.replace("W+jets", "$W$+jets")
        temp_line = temp_line.replace("Z+jets", "$Z$+jets")
        temp_line = temp_line.replace("Data/MC", "DataMC")
        temp_line = temp_line.replace("R/F", "RF")
        temp_line = temp_line.replace("DR/DS", "DRDS")
        temp_line = temp_line.replace("#eta", "$\\eta$")
        temp_line = temp_line.replace("#mu", "$\\mu$")
        temp_line = temp_line.replace("#rho", "$\\rho$")
        temp_line = temp_line.replace("#geq", "$\geq$")
        temp_line = temp_line.replace("#scale[0.75]","")        
        temp_line = temp_line.replace("_{T}","$_{T}$")        
        temp_line = temp_line.replace("_{R}","$_{R}$")        
        temp_line = temp_line.replace("_{F}","$_{F}$")        
        temp_line = temp_line.replace("_{RF}","$_{RF}$")        
        temp_line = temp_line.replace("_{R,F}","$_{R,F}$")        
        temp_line = temp_line.replace("sVLQ_","sVLQ ")        

        #Replace characters
        temp_line=temp_line.replace("|","&")
        temp_line=temp_line.replace("\n","")
        # temp_line=temp_line.replace("_"," ")
        temp_line=temp_line.replace("pm","\\pm")
        temp_line=temp_line.replace("#","\\")
        temp_line = temp_line.rstrip('&')
        temp_line = temp_line[2:]

        #Changing style
        splitted = temp_line.split("&")
        newLine = ""
        for part in splitted:
            part = part[1:]
            newPart=""
            if part.replace(" ","")=="":
                newLine += "&"
                continue

            if isSyst and part.find("/")>-1:#typically the uncertainty values
                part=part.replace(" ","")
                part_splitted=part.split("/")
                up=float(part_splitted[0])*100
                down=float(part_splitted[1])*100
                if abs(abs(up)-abs(down))<0.1:
                    newPart="$ \pm %.1f $"%(abs(up))
                else:
                    if up>0 and down<0:
                        newPart="$ +%.1f / %.1f $"%(up,down)
                    elif up<0 and down>0:
                        newPart="$ %.1f / +%.1f $"%(up,down)
                    else:
                        newPart="$ %.1f / %.1f $"%(up,down)
            elif (not isSyst) and part.find("pm")>-1:
                part=part.replace(" ","")
                part_splitted=part.split("\\pm")
                nom=float(part_splitted[0])
                unc=float(part_splitted[1])
                #roundnom, roundunc = pdgRounding.pdgRound(nom,unc)
                #print "Nominal = ",nom,"Error = ",unc
                #print "Rounded nom = ",roundnom,"Rounded Error = ",roundunc
                newPart="  $ %.2f \pm %.2f $"%(nom,unc)
            #elif part.find("\\")>-1:
            #    part_splitted=part.split(" ")
            #    newPart = ""
            #    for splitted_again in part_splitted:
            #        if splitted_again.find("\\")>-1:
            #            splitted_again = splitted_again.replace("bb","b\\bar{b}")
            #            splitted_again = splitted_again.replace("cc","c\\bar{c}")
            #            splitted_again = splitted_again.replace("light","\\text{light jets}")
            #            if splitted_again.find("makecell")==-1:
            #                newPart += " $"+splitted_again+"$"
            #            else:
            #                newPart = splitted_again
            #        else:
            #            newPart += " " + splitted_again
            else:
                newPart = part

            newLine+=newPart+"&"

        if isSyst:
            newLine=newLine[:(len(newLine)-1)]
        else:
            newLine=newLine[:(len(newLine)-2)]

        columns_splitted = newLine.split("&")
        sample_name = columns_splitted[0]

        result+= [{'sample':sample_name,'columns':columns_splitted}]

    return result

##_______________________________________________________________
##
def getRegionLabel(inFile):
    label = ""
    region = inFile[inFile.rfind('/'):inFile.rfind('.')].replace("_VR","").replace("_postFit","").replace("_syst","")

    if "Yields" in region:
        label = "Total yields"

    if "call" in region:
      label = "No selection"
  
    if '3_5jwin' in region:
      label+='LJ'
    if '6jin' in region:
      label+='HJ'

    if 'bwin' in region:
      label+=', '+region[region.index('bwin')-3]+'-'+region[region.index('bwin')-1]+'b'

    elif 'b' in region:
      label+=', '+region[region.index('b')-1]+'b'
    if 'fj' in region:
      label+=', '+region[region.index('fj')-1]+'fj'

    if 'TH' in region and region[region.index('TH')-1].isdigit():
      label+=', '+region[region.index('TH')-1]+'h+t$_{h}$'
    elif 'T' in region and region[region.index('T')-1].isdigit():
      label+=', '+region[region.index('T')-1]+'t$_{h}$'

    if 'LT' in region and region[region.index('LT')-1].isdigit():
      label+=', '+region[region.index('LT')-1]+'(t$_{h}$+t$_{l}$)'
    elif 'L' in region and region[region.index('L')-1].isdigit():
      label+=', '+region[region.index('L')-1]+'t$_{l}$'

    if 'H' in region and region[region.index('H')-1].isdigit():
      label+=', '+region[region.index('H')-1]+'h'
  
    if 'VLTH' in region:
      label+=', '+region[region.index('VLTH')-1]+'(v+h+t$_{l}$+t$_{h}$)'
    elif 'VLT' in region:
      label+=', '+region[region.index('VLT')-1]+'(v+t$_{l}$+t$_{h}$)'
    if 'VT' in region:
      label+=', '+region[region.index('VT')-1]+'(v+t$_{h}$)'

    if 'V' in region and region[region.index('V')-1].isdigit() and not 'v' in label:
      label+=', '+region[region.index('V')-1]+'V'

    return label

##_______________________________________________________________
##

# input argument is TRExFitter output folder (containing config files and "Results" folder)
inputDir = sys.argv[-1]

# use names in config files to collect all output dirs in Results
outputNames = [n.replace(n[:n.index("/configFile_sVLQ_")+17],"").replace(".txt","") for n in glob.glob(inputDir+"/configFile_sVLQ_*")]
outputDirs = [d for d in glob.glob(inputDir+"/Results/*") if not "scripts" in d and any(n in d for n in outputNames)]

removeSignal=True
separateTables=False
catTogether=True # only set to True if separateTables = False
labelEachTable=True
pdfLatex = True
verbose = False

# retrieve all text files in input folder
allTextFiles = []
for d in outputDirs:

    organized = False
    if os.path.isdir(d+'/Tables/VR/') and len(os.listdir(d+'/Tables/VR/'))>0:
        organized = True

    if organized:
        allTextFiles.extend(glob.glob(d+'/Tables/SR/*txt'))
        allTextFiles.extend(glob.glob(d+'/Tables/VR/*txt'))
    else:
        allTextFiles.extend(glob.glob(d+'/Tables/*txt'))


for textFile in allTextFiles:
    print"-> ",textFile
    inFile = open(textFile,"read")
    outFile = open(textFile.replace(".txt",".tex"),"write")
    writePreamble(inFile,outFile)
    convertAllLines(inFile,outFile)
    writeEnd(inFile,outFile)

    if pdfLatex and separateTables:
        os.system("pdflatex -interaction nonstopmode -halt-on-error -file-line-error %s"%textFile.replace(".txt",".tex"))

if catTogether:
    for outFolder in outputDirs:
        if organized:
            os.system("cat %s/Tables/SR/*_syst.tex > %s/Tables/Syst_tables_SR.tex"%(outFolder,outFolder))
            os.system("cat %s/Tables/SR/*_syst_postFit.tex > %s/Tables/Syst_tables_SR_postFit.tex"%(outFolder,outFolder))
            os.system("cat %s/Tables/VR/*_syst.tex > %s/Tables/Syst_tables_VR.tex"%(outFolder,outFolder))
            os.system("cat %s/Tables/VR/*_syst_postFit.tex > %s/Tables/Syst_tables_VR_postFit.tex"%(outFolder,outFolder))
        else:
            os.system("cat %s/Tables/*in_syst.tex %s/Tables/*ex_syst.tex > %s/Tables/Syst_tables_SR.tex"%(outFolder,outFolder,outFolder))
            os.system("cat %s/Tables/*in_syst_postFit.tex %s/Tables/*ex_syst_postFit.tex > %s/Tables/Syst_tables_SR_postFit.tex"%(outFolder,outFolder,outFolder))
            os.system("cat %s/Tables/*VR_syst.tex > %s/Tables/Syst_tables_VR.tex"%(outFolder,outFolder))
            os.system("cat %s/Tables/*VR_syst_postFit.tex > %s/Tables/Syst_tables_VR_postFit.tex"%(outFolder,outFolder))

        allSystTables = [t for t in glob.glob(outFolder+'/Tables/Syst_tables_*.tex')]
        # allSystTables.extend([y for y in glob.glob(outFolder+'/Tables/Yields*.tex')])

        for tab in allSystTables:
            writePreamble(open(tab,"r"),open(tab,"r"))
            writeEnd(open(tab,"a"),open(tab,"a"))
    
            if pdfLatex:
                os.system("pdflatex -interaction nonstopmode -output-directory=%s %s "%(tab[:tab.rfind('/')],tab))
                os.system("rm %s"%tab.replace('.tex','.log'))
                os.system("rm %s"%tab.replace('.tex','.aux'))
