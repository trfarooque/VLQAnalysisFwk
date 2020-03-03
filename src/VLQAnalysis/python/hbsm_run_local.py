#!/bin/env python

import re
import utils

def main():
    sample_fullname='user.farooque.410000.PowhegPythiaEvtGen.DAOD_TOPQ1.e3698_s2608_s2183_r7267_r6282_p2516.HtX4Tops_00-00-10_output.root'
    sample_shortname='ttbar' # this must match the existing convention used for trex: is light/bb/cc made from 'ttbar'?

    sample_fullname='user.gerbaudo.343434.MadGraphPythia8EvtGen.DAOD_TOPQ1.e4776_a766_a810_r6282_p2454.HtX4Tops_00-00-10_output.root'
    sample_shortname='bbH_m1000'
    # sample_fullname='user.farooque.410000.PowhegPythiaEvtGen.DAOD_TOPQ1.e3698_s2608_s2183_r7267_r6282_p2516.HtX4Tops_00-00-10_output.root'
    # sample_shortname='ttbar'
    output_dir = utils.mkdir_if_needed('local/output/'+sample_shortname)
    log_file = output_dir+'/run.txt'
    run_sample(sample_fullname=sample_fullname, sample_shortname=sample_shortname,
               output_dir=output_dir, log_file=log_file)
    


# todo
# dsids=( 343431 343432 343433 343434 )
# masses=( A_m500 A_m1000 H_m500 H_m1000 )
# suffix="tanb10"
# 
# dsids=( 343434 )
# masses=( H_m1000 )
# 
# dsids=(  343432 343434 )
# masses=( A_m1000 H_m1000 )
# 
# # 2016-04-01 look at 4t2HDM
# dsids=( 304777 304780 )
# masses=( 4t2HMD_400 4t2HMD_700 )
# suffix="tanb1"
# 
# dsids=( 304777 )
# masses=( 4t2HMD_400 )
# suffix="tanb1"
# 
# # 2016-04-18 look at h+
# dsids=( 341541 )
# masses=( Hplus4FS_H200 )
# suffix="tb"


def run_sample(sample_fullname='', sample_shortname='', output_dir='', log_file='foo.txt'):

    filelist = "VLQAnalysis/data/hbsm/filelist/eos/{sample_fullname:s}.txt".format(**{'sample_fullname':sample_fullname})
    filelist = "VLQAnalysis/data/hbsm/filelist/at3pnfs/{sample_fullname:s}.txt".format(**{'sample_fullname':sample_fullname})
    # filelist = "VLQAnalysis/data/hbsm/filelist/at2scratch2/{sample_fullname:s}.txt".format(**{'sample_fullname':sample_fullname})

    dsid = utils.guess_id_from_name(sample_fullname)
 # --nEvents=10 \\
 # --msgLevel=DEBUG \\
    cmd = """VLQAnalysis \\
 --outputFile={output_dir:s}/{sample_fullname:s}.root \\
 --inputFile={input_file:s} \\
 --textFileList=true \\
 --sampleName=hbsm \\
 --weightConfigs=${{VLQAnalysisFramework_DIR}}/data/VLQAnalysis/list_weights.list \\
 --doOneLeptonAna=true \\
 --useLeptonsSF=true \\
 --useLeptonTrigger=true \\
 --RECOMPUTETTBBRW=false \\
 --RWTTFRACTIONS=false \\
 --filterType=NOFILTER \\
 --doTRF=false \\
 --btagOP=77 \\
 --doBlind=false \\
 --useMiniIsolation=false \\
 --usePUWeight=false \\
 --splitVLQDecays=true \\
 --dumpHistos=true \\
 --applyMetMtwCuts=false \\
 --doTruthAnalysis=false \\
 --sampleID={dsid:s}. \\
 --inputTree=nominal_Loose \\
 --isData=false \\
 --computeWeightSys=true \\
 2>&1 | tee  {log_file:s}
"""
 # >& {log_file:s}
    cmd = cmd.format(**{
            'output_dir' : output_dir,
            'sample_fullname' : sample_fullname,
            'input_file' : filelist,
            'sample_name' : sample_shortname,
            'dsid' : dsid,
            'log_file' : log_file
            })
    print "Running the following command:\n%s" % cmd    
    cmd_out = utils.get_command_output(cmd)
    if cmd_out['returncode']:
        print 'command failed.'

if __name__=='__main__':
    main()
