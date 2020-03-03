#!/bin/bash

# template to submit VLQ jobs on the at3 cluster
#
# Note to self:
# if you want to use curly braces to highlight bash variables, you
# need to escape them with double curly (otherwise they're picked up
# by the python string.format
#
# Variable replacement strategy:
# - replace variables are early as possible, to make commands in
#   resulting scritps explicit (--> more easily 'joinable')
# - when there is a value repeated several times, declare a local
#   variable within the function
# - assume mininal env vars (i.e. only LSB_JOBID)
# Log file strategy:
# - keep separate log files for compilation and processing, since we
#   might want to join samples
# - keep explicit logfile names within each function (easier joining)

echo "setting up root "
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${{ATLAS_LOCAL_ROOT_BASE}}/user/atlasLocalSetup.sh

function prepare() {{
    local absolute_base_dir="{absolute_base_dir:s}"
    echo "Current directory: `pwd`"
    echo "Current contents:"
    ls -ltrh
    cp -p {tar_file:s} ./tarball.tgz
    echo "untar tarball.tgz"
    ls -lh tarball.tgz
    tar xzf tarball.tgz
    echo "Starting 'compilation' step `date`"
    rc find_packages
    rc clean
    rc compile
    echo "Completed 'compile' step `date`"
}}

function run() {{
    echo "Processing {sample_name:s} `date`"
    mkdir -p {relative_log_dir:s}
    mkdir -p {relative_output_dir:s}
    local subtask_log_file={relative_log_dir:s}/run_{job_label:s}.log
    VLQAnalysis \
 --outputFile={relative_output_dir:s}/{output_file:s} \
 --inputFile={filelist_name:s} \
 --textFileList=true \
 --sampleName={sample_name:s} \
 --weightConfigs=${{VLQAnalysisFramework_DIR}}/data/VLQAnalysis/list_weights.list \
 --doOneLeptonAna=true \
 --useLeptonsSF=true \
 --useLeptonTrigger=true \
 --RECOMPUTETTBBRW=false \
 --RWTTFRACTIONS=false \
 --filterType=NOFILTER \
 --doTRF=false \
 --btagOP=77 \
 --doBlind=false \
 --useMiniIsolation=false \
 --usePUWeight=false \
 --splitVLQDecays=true \
 --dumpHistos=true \
 --applyMetMtwCuts=false \
 --doTruthAnalysis=false \
 --sampleID={dsid:s}. \
 --inputTree={input_tree:s} \
 --isData=false \
 --computeWeightSys={compute_weight_sys:s} \
 --onlyDumpSystHistograms=true \
 {other_options:s} \
 >> ${{subtask_log_file}} 2>&1 || true

# These options are only for tests
# --msgLevel=DEBUG \  # can lead to large logfiles
# --nEvents=10 \      # for test

    if test -e {relative_output_dir:s}/{output_file:s}
        then
        echo "files being copied: [begin]"
        ls -ltrh {relative_output_dir:s}/*
        echo "files being copied: [end]"
        rsync -az {relative_log_dir:s}/* {absolute_base_dir:s}/{relative_log_dir:s}
        rsync -az {relative_output_dir:s}/* {absolute_base_dir:s}/{relative_output_dir:s}
        touch "{absolute_base_dir:s}/{relative_status_dir:s}/{job_label:s}.done.${{LSB_JOBID}}"
        else
        touch "{absolute_base_dir:s}/{relative_status_dir:s}/{job_label:s}.fail.${{LSB_JOBID}}"
    fi
    echo "Processed {sample_name:s} `date`"

}}

function main() {{
    echo "Start `date`"
    cd ${{TMPDIR}}
    echo "working from `pwd`"
    echo "Setting up release:"
    lsetup 'rcSetup -u'
    lsetup 'rcsetup {rc_release_version:s}'
    prepare
    run
    echo "Cleaning up"
    rm -rf $TMPDIR/*
}}

main
