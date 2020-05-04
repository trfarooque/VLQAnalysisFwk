#!/bin/python
import glob
import math
#import pdgRounding

removeSignal=True

##_______________________________________________________________
##
def writePreamble( inFile, fout ):
    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True
    if not isSyst:
        fout.write("\\begin{table}\n")
        fout.write("\\begin{center}\n")

##_______________________________________________________________
##
def writeEnd(fout):
    isSyst = False
    if inFile.name.find("syst")>-1: isSyst = True
    if not isSyst:
        fout.write("\\end{center}\n")
        fout.write("\\end{table}\n")

##_______________________________________________________________
##
def convertAllLines( inFile, fout ):

    isSyst = False
    #if inFile.name.find("syst")>-1: isSyst = True

    #gets all results properly formatted
    result = dumpVector(inFile)

    #nSplit
    n_split = 4
    if(isSyst): n_split = 1000

    #n_tables
    n_tables = 1

    #gets the final table
    if len(result[0]['columns'])>n_split and (not isSyst):
        n_tables = int(math.ceil((len(result[0]['columns'])-1)/float(n_split)))
    elif isSyst:
        nSplit = len(result[0]['columns'])-1

    print 'inFile.name ', inFile.name
    print 'n_split ', n_split
    print 'n_tables ', n_tables
    print 'isSyst ', isSyst


    for i_table in range(n_tables):
        if not isSyst:
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

            if count_sample==0 or (not isSyst and (count_sample==len(result)-2)):
                fout.write("    \\hline \n")
            count_sample+=1
        fout.write("    \hline \hline \n")

        if not isSyst:
            fout.write("  \end{tabular} \n")

        fout.write("\n\n")

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
        temp_line = temp_line.replace("#geq", "$\geq$")
        temp_line = temp_line.replace("}{", " \\\\ ")
        temp_line = temp_line.replace("t_{l}", "$t_{l}$")
        temp_line = temp_line.replace("t_{h}", "$t_{h}$")
        temp_line = temp_line.replace("t#bar{t}+light", "$t\\bar{t}+\geq 1l$")
        temp_line = temp_line.replace("t#bar{t}+cc", "$t\\bar{t}+\geq 1c$")
        temp_line = temp_line.replace("t#bar{t}+bb", "$t\\bar{t}+\geq 1b$")
        temp_line = temp_line.replace("W+jets", "$W$+jets")
        temp_line = temp_line.replace("Z+jets", "$Z$+jets")
        
        #Replace characters
        temp_line=temp_line.replace("|","&")
        temp_line=temp_line.replace("\n","")
        temp_line=temp_line.replace("_"," ")
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


#allTextFiles=glob.glob("BackgroundOnly_10FB_1LeptonOnly/Tables/*.txt")
#/nfs/at3/scratch2/farooque/VLQFITINPUTS/2017_05_09/ExotCirc/Unblinded/ProjectedPostFitsPlots/Outputs/PaperTablesPostFit/PaperTables_Bkgd_1L/Tables/Yields*.txt
allTextFiles=glob.glob("/nfs/at3/scratch2/farooque/VLQFITINPUTS/VLQ2018/sVLQ/tag21.2.87-3-syst/FitSys//Results/BONLY__sVLQ_WTHt11K03__/Tables/*.txt")
for textFile in allTextFiles:
    print"-> ",textFile
    inFile = open(textFile,"read")
    outFile = open(textFile.replace(".txt",".tex"),"write")
    writePreamble(inFile,outFile)
    convertAllLines(inFile,outFile)
    writeEnd(outFile)
