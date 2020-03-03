#!/bin/env python

# davide.gerbaudo@gmail.com
# Jun 2016

import glob
import optparse
import os

import batch_utils
import sample_catalogues
import systematics
import utils

description = """
Submit to batch the jobs to run VLQAnalysis/util/VLQAnalysis.cxx
"""

usage = """
First time from a new area:
%prog -l bkgonly_2016-05-25 --generate-groupfiles

Then:

# prepare scripts
%prog -l test_2016-10-19
# just nominal, only hbsm samples, specify queue
%prog -l test_2016-10-19 --groupfile VLQAnalysis/data/groups/hbsm/hbsm.txt  --syst nominal --queue 8nh --verbose
# command to test the code locally on one sample
%prog -l test_2016-10-27  --groupfile VLQAnalysis/data/groups/hbsm/hbsm.txt --sample-include 341541 --print-local
# check the output root files
%prog -l test_2016-10-19 --groupfile VLQAnalysis/data/groups/hbsm/hbsm.txt  --syst nominal --queue 8nh --check

(run with -h to get all options)

This script should be called from the directory above VLQAnalysis and RootCoreBin.

You need to provide a label for this batch of jobs; this script will
- create the appropriate directory structure in batch/input, batch/output
- starting from a job template, generate the job scripts
- submit the jobs

The rules to go from the templates to the job-specific files are:
- python variables are specified as '{varname:s}', bash variables are
  specified as '${varname}'
- python variables are replaced with their values by this script, bash
  variables are evaluated at runtime
- if you get a 'KeyError' when filling the template, it means you need
  to escape the python replacement:
  Example: 'E_{T}{something}' -> 'E_{{T}}{{something}}'
  This can be done either in the template or in this script.

TODO split large jobs
TODO merge split outputs
"""

def main():

    parser = optparse.OptionParser(description=description, usage=usage)
    parser.add_option('-l', '--label', default=None, help='job label; used to make input/output subdirectories')
    parser.add_option('-q', '--queue', default=None)
    # TODO fix syst option
    parser.add_option('-s', '--syst', default='nominal', help="variations to process ('weight', 'object', default %default).")
    parser.add_option('--list-systematics', action='store_true', help='list the systematics available in the catalogue')
    # parser.add_option('--syst-include', default='.*', help='include only the systematics matching the regexp')
    # parser.add_option('--syst-exclude', default=None, help='exclude the systematics matching the regexp')
    parser.add_option('--check', action='store_true', help='check the root output files')
    parser.add_option('--resubmit', action='store_true', help='resubmit failed jobs')
    # TODO the sample filtering works only with the --overwrite-scripts option?
    parser.add_option('--sample-include', default='.*', help='include only the samples matching the regexp (short name if available, else full_name)')
    parser.add_option('--sample-exclude', default=None, help='include only the samples matching the regexp (short name if available, else full_name)')
    parser.add_option('-S', '--submit', action='store_true', help='actually submit the jobs')
    parser.add_option('--batch-template', help='batch template; otherwise use default one from JobManager')
    parser.add_option('--tarfile', default=None, help='the tar file will contain the code')
    parser.add_option('--overwrite-tar', action='store_true', help='re-create tar even when it exists')
    parser.add_option('--overwrite-scripts', action='store_true', help='re-create the batch scripts even when they exist')
    parser.add_option('--generate-groupfiles', action='store_true', help='generate group files')
    parser.add_option('--generate-filelists', action='store_true', help='generate input file lists')
    parser.add_option('--groupfile', default=None, help='if you just want to run on one group file, eg. data/groups/hbsm/hbsm.txt')
    parser.add_option('--input-from', default='rucioeos',
                      help='Where the ntuples are stored; see sample_catalogues.InputDataInterface')
    parser.add_option('--merge-fewer', default=1, type=int, help='merge jobs if less than N input files')
    parser.add_option('--split-larger', default=500, type=int, help='split jobs if more than N input files (default %default)')
    parser.add_option('--print-local', action='store_true', help='print the command to run locally')
    parser.add_option('-v', '--verbose', action='store_true', help='print what it is doing')
    parser.add_option('-d', '--debug', action='store_true', help='print even more debugging information')

    (opts, args) = parser.parse_args()
    if not opts.label:
        parser.error('You must provide a label option')
    if opts.label and opts.label[0].isdigit():
        parser.error('Label cannot start with a digit')
    if not is_valid_input(opts):
        parser.error('Invalid --input-from')
    if opts.resubmit and opts.overwrite_scripts:
        parser.error('These two options are not compatible: --resubmit --overwrite-scripts')
    if opts.list_systematics:
        systematics.SystematicCatalogue().print_all()
        return

    batch_platform = batch_utils.guess_batch_platform()
    job_manager = (batch_utils.At3JobManager if batch_platform=='at3' else
                   batch_utils.LxbJobManager)
    job_manager = job_manager(jobset_label=opts.label, verbose=opts.verbose, debug=opts.debug,
                              overwrite_batch_scripts=opts.overwrite_scripts)
    job_manager.dry_run = not opts.submit
    if opts.queue: job_manager.queue = opts.queue
    if opts.batch_template: job_manager.template_path = opts.batch_template
    if opts.overwrite_tar: job_manager.overwrite_tar = True
    if opts.verbose:
        utils.print_running_conditions(parser, opts)
    if opts.debug:
        utils.print_parsed_options(parser, opts)

    if opts.generate_groupfiles:
        sample_catalogue = sample_catalogues.HbsmSampleCatalogue() # TODO or VlqSampleCatalogue
        # TODO prompt: ask about sample list from new production
        sample_catalogue.add_samples_from_file(path='VLQAnalysis/data/samples_HtX4TopsNtuple-MBJ-2.4.24-1-0.txt')
        sample_catalogue.categorise_samples(sample_catalogue.samples)
        sample_catalogue.write_group_files()
        return
    if job_manager.needs_to_generate_scripts:
        if opts.verbose:
            print "Need to generate scripts: gathering samples and filelists"
        sample_catalogue = sample_catalogues.HbsmSampleCatalogue() # TODO or VlqSampleCatalogue
        sample_catalogue.add_samples_from_group_files(glob.glob(opts.groupfile) if opts.groupfile else
                                                      glob.glob(sample_catalogue.groupfiles_directory+'/*.txt'))
        sample_catalogue.prune_samples(regex_include=opts.sample_include,
                                       regex_exclude=opts.sample_exclude)
        samples_to_process = (sample_catalogue.samples if opts.syst=='nominal' else
                              sample_catalogue.add_systematic_variations(samples=sample_catalogue.samples,
                                                                         verbose=opts.verbose,
                                                                         syst_option=opts.syst))

        if opts.generate_filelists:
            pass
        input_interface = (sample_catalogues.RucioEosCernInterface() if opts.input_from=='rucioeos' else
                           sample_catalogues.EosUserInterface() if opts.input_from=='eosuser' else
                           sample_catalogues.RucioPnfsIfaeInterface() if opts.input_from=='ruciopnfs' else
                           sample_catalogues.At3ScratchDiskInterface() if opts.input_from=='at3disk' else
                           sample_catalogues.LocalDiskInterface('VLQAnalysis/data/filelist/', base_input_dir=opts.input_from))
        input_interface.attach_filelists(samples=samples_to_process, verbose=opts.verbose)

        # samples_to_process = sample_catalogue.add_filelists(samples=samples_to_process, input_interface=input_interface)

        for sample in samples_to_process:
            for systematic in sample.systematic_uncertainties:
                for variation in systematic.variations:
                    job_manager.create_job(sample, systematic, variation)
        if opts.split_larger>2:
            job_manager.split_jobs(split_every_n=opts.split_larger)
        if opts.merge_fewer>1:
            job_manager.merge_jobs(min_n_input_files=opts.merge_fewer)
    else:
        if opts.verbose:
            print "JobManager: using existing scripts from %s" % job_manager.relative_input_directory
        # TODO (perhaps: need to think about expected behaviour) filter samples?
        # Q: how would you filter on merged samples? on the jobset name or on the sample name
    if opts.print_local:
        if not all(type(j) is batch_utils.Job for j in job_manager.jobs):
            raise NotImplementedError("This feature is available only for non-merged jobs; drop the --merge-fewer option")
        print 'Commands to test the code locally:'
        print '\n'.join(batch_utils.Job.parse_run_cmd_line(j.script_path) for j in job_manager.jobs)
        return
    if opts.check:
        job_manager.check_outputs()
    elif opts.resubmit:
        job_manager.resubmit_failed_jobs()
    else:
        job_manager.submit_jobs()


def is_valid_input(opts):
    "either you specified a predifined input interface, or a directory"
    return (opts.input_from in ['at3disk', 'eosuser', 'rucioeos', 'ruciopnfs'] or
            os.path.isdir(opts.input_from))


if __name__=='__main__':
    main()
