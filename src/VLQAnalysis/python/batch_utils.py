"""
This module provides utilities to interact with batch systems (lxplus or at3)

Overall design:

JobManager creates Jobs and JobSets
Jobs    can be generated/submitted/checked/resubmitted
JobSets can be generated/submitted/checked/resubmitted

Each group of jobs has a label; all relevant files go in the directories below:

batch/
`--- <label>/
     |--- input/
     |--- log/
     |--- output/
     `--- status/

Each sample will produce a status file sample.[done,fail] and a root file.

TODO: to avoid having too many files in one place, think about having
      subdirectories under output (for example by group + merged)

davide.gerbaudo@gmail.com
Jul 2016
"""

# import copy
import glob
import os
# import re
import collections

from VLQAnalysis import utils
from VLQAnalysis.sample_catalogues import HbsmSampleCatalogue, SampleCatalogue, LocalDiskInterface, RucioEosCernInterface

#___________________________________________________________


def base_directory():
    """The base directory is the one above VLQAnalysis and RootCoreBin

    All relatives paths are relative to base_directory or to the
    working directory on the batch node.
    """
    python_dir = os.path.dirname(os.path.abspath(__file__))
    up_two = (os.pardir, os.pardir)
    return os.path.normpath(os.path.abspath(os.path.join(python_dir, *up_two)))

def vlq_directory():
    return base_directory()+'/VLQAnalysis'

#___________________________________________________________

class Job(object):
    """A job with a script file and an expected ouput.
    """
    keyword_output_file = 'outputFile='
    keyword_input_line = 'inputFile='
    keyword_run_function = 'function run() {'
    keyword_main_function = 'function main() {'
    keyword_exe_line = 'VLQAnalysis'
    def __init__(self, script_path, nickname=None):
        self.script_path = script_path
        self._expected_output_file = None
        self._number_input_files = None
        self._nickname = nickname # to get reasonable names for split jobs; otherwise = basename(script_path)

    @property
    def nickname(self):
        "use to name the status and log files"
        return (self._nickname if self._nickname else
                utils.filename_without_extension(self.script_path)) # w/out ext, =sample name
    @classmethod
    def parse_expected_output_file(cls, file_path):
        cmd = "grep %s %s" % (Job.keyword_output_file, file_path)
        out = utils.get_command_output(cmd)
        tokens = out['stdout'].strip().replace('=', ' ').split()
        output_file = file_path = next((t for t in tokens if '.root' in t), '')
        output_file = output_file.strip()
        return output_file

    @classmethod
    def parse_input_file(cls, file_path):
        cmd = "grep %s %s" % (Job.keyword_input_line, file_path)
        out = utils.get_command_output(cmd)
        input_file = out['stdout'].split('=')[1].strip().split()[0]
        return input_file

    @classmethod
    def parse_run_cmd_line(cls, file_path):
        run_function = JobSet.extract_function(file_path, Job.keyword_run_function)
        run_line     = ' '.join(l for l in utils.drop_continuation_lines(run_function.split('\n'))
                                # if Job.keyword_run_function in l # TODO I think should be needed, but it works without ?????
                                )
        return run_line

    @classmethod
    def has_multiple_configuration_lines(cls, input_filelist):
        number_of_config_blocks = 0
        in_config_block = False
        keyword = SampleCatalogue.keyword_job_option
        for line in SampleCatalogue.read_lines_from_txt(input_filelist,
                                                        keywords_useful_comment_line=[keyword]):
            if in_config_block and keyword in line:
                continue
            elif keyword in line:
                in_config_block = True
                number_of_config_blocks += 1
            else:
                in_config_block = False

        return number_of_config_blocks>1

    @property
    def expected_output_file(self):
        if not self._expected_output_file:
            self._expected_output_file = self.parse_expected_output_file(file_path=self.script_path)
            if not self._expected_output_file:
                raise RuntimeError("cannot extract output file from %s" % self.script_path)
        return self._expected_output_file
    @property
    def expected_output_files(self):
        "Just to provide the same interface as JobSet"
        return [self.expected_output_file]
    @property
    def number_input_files(self):
        if not self._number_input_files:
            cmd = 'grep "inputFile=" '+self.script_path
            out = utils.get_command_output(cmd)
            filelist_path = next((t for t in out['stdout'].split() if 'inputFile' in t), None) # find inputFile keyword
            filelist_path = filelist_path.split('=')[1].strip() # take what follows =
            filelist_path = filelist_path.split()[0].strip() # and drop any subsequent words
            self._number_input_files = sum(1 for l in open(filelist_path).readlines() if not l.strip().startswith('#'))
        return self._number_input_files
    @classmethod
    def is_completed(cls, job):
        "make it a classmethod so that we can use it also for JobSet"
        out_filenames = job.expected_output_files
        expect = len(out_filenames)
        done = sum(1 for f in out_filenames if os.path.exists(f))
        return expect==done

#___________________________________________________________

class JobSet(object):
    """A job with a script file and multiple expected ouputs.

    A jobset can be built either merging several jobs or from a merged script.
    """
    def __init__(self,script_path=None, jobs=[], verbose=False):
        self.script_path = None
        self._expected_output_files = []
        if not script_path:
            raise NotImplementedError("JobSet requires a script path")
        from_merged_script = os.path.exists(script_path) and not jobs
        from_jobs_to_merge = jobs and len(jobs)>0
        if from_merged_script==from_jobs_to_merge:
            raise NotImplementedError("Invalid arguments: script_path %s, %d jobs"%(script_path,
                                                                                    len(jobs)))
        if from_merged_script:
            self.script_path = script_path
            self._expected_output_files = JobSet.parse_expected_output_files(script_path)
        else:
            pre_merge_script_paths = [j.script_path for j in jobs]
            self.script_path = JobSet.merge_scripts(orig_job_script_paths=pre_merge_script_paths,
                                                    dest_script_path=script_path)
            os.chmod(self.script_path, 0755)
            JobSet.delete_pre_merge_scripts(file_paths=pre_merge_script_paths)
            self._expected_output_files = JobSet.parse_expected_output_files(script_path)
            if verbose:
                print "merged into\n > %s\nfrom%s" % (script_path, '\n< '.join(['']+pre_merge_script_paths))

    @property
    def expected_output_files(self):
        "No need to cache here: the constructor should extract from the script file (which is always there)"
        if not self._expected_output_files:
            raise RuntimeError("something went wrong when parsing %s?" % self.script_path)
        return self._expected_output_files

    @classmethod
    def parse_expected_output_files(cls, file_path, expected_keyword='outputFile='):
        "same as Job.parse_expected_output_file, but with multiple output files"
        cmd = "grep %s %s" % (expected_keyword, file_path)
        out = utils.get_command_output(cmd)
        lines = out['stdout'].split('\n')
        filenames = []
        for line in lines:
            tokens = line.strip().replace('=', ' ').split()
            output_file = file_path = next((t for t in tokens if '.root' in t), '')
            if output_file and output_file.strip():
                filenames.append(output_file.strip())
        return filenames

    @classmethod
    def merge_scripts(cls, orig_job_script_paths=[], dest_script_path=None):
        """merge the batch job scripts.

        Modify the first script and plug in the 'run' functions from
        all the subsequent ones. The subsequent scripts are deleted
        after the merge.
        """
        template_path = orig_job_script_paths[0]
        template_contents = open(template_path).read()
        run_fuction   = JobSet.extract_function(template_path, Job.keyword_run_function)
        main_function = JobSet.extract_function(template_path, Job.keyword_main_function)

        run_fuctions = [JobSet.extract_function(j, Job.keyword_run_function) for j in orig_job_script_paths]
        run_fuctions = [f.replace(Job.keyword_run_function,
                                  Job.keyword_run_function.replace('run', "run%02d"%i))
                        for i, f in enumerate(run_fuctions)]
        main_function_new = '\n'.join(l if not l.strip()=='run' else
                                      '\n'.join(l.replace('run', "run%02d"%i)
                                                for i in range(len(run_fuctions)))
                                      for l in main_function.split('\n'))
        script_contents = (template_contents
                           .replace(main_function, main_function_new)
                           .replace(run_fuction,
                                    '\n'.join(run_fuctions)))
        with open(dest_script_path, 'w') as output_script:
            output_script.write(script_contents)
        return dest_script_path

    @classmethod
    def delete_pre_merge_scripts(cls, file_paths=[]):
        for f in file_paths:
            os.remove(f)

    @classmethod
    def extract_function(cls, script_path, starting_token):
        """extract the 'run' bash function from the script

        Note to self: cannot use regex because they cannot parse nested structures.
        """
        if not ('function' in starting_token and '{' in starting_token):
            raise NotImplementedError("invalid starting_token, must contain 'function and ''{': \n''%s'"%starting_token)
        function_lines = []
        with open(script_path) as input_file:
            contents = input_file.read()
            if starting_token in contents:
                begin_pos = contents.find(starting_token)
                contents = contents[begin_pos:]
                scope_counter = 0
                for line in contents.split('\n'):
                    if line.strip().startswith('#'):
                        function_lines.append(line)
                        continue
                    open_curly_counter = line.count('{')
                    close_curly_counter = line.count('}')
                    scope_counter += (open_curly_counter - close_curly_counter)
                    function_lines.append(line)
                    if scope_counter==0:
                        break
        return '\n'.join(function_lines)

    @property
    def nickname(self):
        "use to name the status and log files"
        return utils.filename_without_extension(self.script_path) # w/out ext

#___________________________________________________________

class JobSplit(object):
    """A job split in several subjobs.

    A jobset can be built either splitting an existing job or from several splitted script.

    The input/log/output/status files will be stored in the
    'split/nnn/mmm' subdirectories of each directory used by
    JobManager, where nnn is a counter of the JobSplit for this
    session (i.e. JobManager lable) and mmm is the index of the
    children job.
    """
    existing_jobsplit_counter = 0 # used to keep track of NN
    def __init__(self, base_job=None, children_jobs=[], job_manager=None, max_n_input_files=10, verbose=False):
        need_to_write_children_scripts = len(children_jobs)==0
        self._expected_output_file = None
        self.requires_merging = True
        if not need_to_write_children_scripts:
            self.parent_job = base_job
            self.children_jobs = children_jobs
        else:
            self.parent_job = base_job
            self.children_jobs = []
            nnn = "%03d" % JobSplit.existing_jobsplit_counter
            input_filelist = Job.parse_input_file(file_path=base_job.script_path)
            filelist_dir = os.path.dirname(input_filelist)
            dirs_to_modify = [job_manager.relative_input_directory,
                              job_manager.relative_log_directory,
                              job_manager.relative_output_directory,
                              job_manager.relative_status_directory]
            script_dir = os.path.dirname(base_job.script_path)
            script_content = open(base_job.script_path).read()
            run_function = JobSet.extract_function(base_job.script_path, Job.keyword_run_function)
            sub_filelists = JobSplit.split_filelist(orig_filelist=input_filelist,
                                                    dest_subdir=os.path.join(filelist_dir, nnn),
                                                    max_n_input_files=max_n_input_files)
            for iJob, sub_filelist in enumerate(sub_filelists):
                mmm = "%03d" % iJob
                job_manager.create_directories(subdir=nnn+'/'+mmm, verbose=False)
                script_dest = utils.mkdir_if_needed(script_dir+'/'+nnn)+'/'+mmm+'.sh'
                new_run_function = run_function.replace(input_filelist, sub_filelist)
                for dtm in dirs_to_modify:
                    new_run_function = new_run_function.replace(dtm, dtm+'/'+nnn+'/'+mmm)
                with open(script_dest, 'w') as of:
                    of.write(script_content.replace(run_function, new_run_function))
                os.chmod(script_dest, 0755)
                self.children_jobs.append(Job(script_dest, nickname=nnn+'_'+mmm))
            if verbose:
                print "split job in %d subjobs (subdir '%s') %s" % (len(self.children_jobs), nnn, self.parent_job.script_path)
            JobSplit.existing_jobsplit_counter += 1

    @classmethod
    def split_filelist(cls, orig_filelist=None, dest_subdir=None, max_n_input_files=10):
        "take a filelist, split it and return a list of smaller ones that have at most N input files"
        if not os.path.exists(orig_filelist) and orig_filelist.endswith('.txt'):
            raise NotImplementedError("This does not look like a txt filelist: %s" % orig_filelist)
        input_lines = []
        with open(orig_filelist) as input_file:
            input_lines = [l.strip() for l in input_file.readlines()]
            if Job.has_multiple_configuration_lines(orig_filelist):
                raise NotImplementedError("Cannot split a filelists containing multiple '# config:' blocks:"
                                          "\n %s; split it." % orig_filelist)
        utils.mkdir_if_needed(dest_subdir)
        sub_job_counter = 0
        sub_job_lines = []
        comment_lines = [l for l in input_lines if l.startswith('#')] # these are propagated to each subfile
        other_lines = [l for l in input_lines if not l.startswith('#')]
        sub_filelists = []
        for line in other_lines:
            sub_job_lines.append(line)
            if len(sub_job_lines) >= max_n_input_files:
                sub_job_filelist = "%s/%03d.txt" % (dest_subdir, sub_job_counter)
                with open(sub_job_filelist, 'w') as of:
                        of.write('\n'.join(comment_lines + sub_job_lines + ['']))
                sub_job_counter += 1
                sub_job_lines = []
                sub_filelists.append(sub_job_filelist)
        if len(sub_job_lines):
            sub_job_filelist = "%s/%03d.txt" % (dest_subdir, sub_job_counter)
            with open(sub_job_filelist, 'w') as of:
                of.write('\n'.join(comment_lines + sub_job_lines + ['']))
            sub_job_counter += 1
            sub_filelists.append(sub_job_filelist)
        return sub_filelists

    @property
    def expected_output_file(self):
        "TODO remove duplication with Job "
        if not self._expected_output_file:
            self._expected_output_file = self.parse_expected_output_file(file_path=self.script_path)
            if not self._expected_output_file:
                raise RuntimeError("cannot extract output file from %s" % self.script_path)
        return self._expected_output_file

    @property
    def expected_output_files(self):
        if self.requires_merging:
            return [j.expected_output_file for j in self.children_jobs]
        else:
            return [self.expected_output_file]


    # TODO
    # @property
    # def expected_output_files(self):
    #     "No need to cache here: the constructor should extract from the script file (which is always there)"
    #     if not self._expected_output_files:
    #         raise RuntimeError("something went wrong when parsing %s?" % self.script_path)
    #     return self._expected_output_files
#___________________________________________________________

class SplitMap(object):
    """Used to keep track of how jobs are split

    Just a list of lines, where in each line the first word is the
    master script_path, and the following ones are the children script
    paths. Mainly useful for __str__.
    """
    def __init__(self, splitjobs=[]):
        self.splitjobs = splitjobs
    def __str__(self):
        return '\n'.join(' '.join([jm.parent_job.script_path]+
                                  [jc.script_path for jc in jm.children_jobs])
                         for jm in self.splitjobs)

#___________________________________________________________

class JobManager(object):
    """Manage a set of jobs; all inputs/outputs will be under batch/<jobset_label>

    A split_map is used to keep track of what job goes into which subjobs.
    """
    def __init__(self, jobset_label, verbose=False, debug=False, overwrite_batch_scripts=False):
        self.jobset = jobset_label
        self.queues = []
        self._queue = None
        self.absolute_base_dir = base_directory()
        self.dry_run = True
        self.jobs = []
        self._tar_file = None
        self.template_path = None
        self._template_contents = None # cached
        self.verbose = verbose
        self.debug = debug
        self.overwrite_tar = False
        self.overwrite_batch_scripts = overwrite_batch_scripts
        self.rc_release_version = guess_rc_release_version()
        self.create_directories()
        existing_scripts = glob.glob(self.relative_input_directory+'/*.sh')
        if existing_scripts and not overwrite_batch_scripts:
            using_sample_scripts = all('jobset' not in f for f in existing_scripts)
            using_merged_scripts = all('jobset' in f for f in existing_scripts)
            using_split_scripts = os.path.exists(self.split_map_file_path)
            if using_merged_scripts==using_sample_scripts:
                raise NotImplementedError("Cannot handle a mix of merged/unmerged scripts from %s" % self.relative_input_directory)
            self.jobs = [JobSet(script_path=f, verbose=self.verbose) if using_merged_scripts else
                         Job(script_path=f)
                         for f in sorted(existing_scripts)]
            if using_split_scripts:
                self.read_split_map()
        elif verbose:
            print "JobManager: you now need to loop over samples/variations and create the jobs"

    def __del__(self):
        self.save_split_map()

    @property
    def split_map_file_path(self):
        return self.relative_status_directory+'/split_map.txt'

    def save_split_map(self):
        splitjobs = self.get_split_jobs()
        if splitjobs:
            split_map = SplitMap(splitjobs=splitjobs)
            with open(self.relative_status_directory+'/split_map.txt', 'w') as of:
                of.write(str(split_map))
            if self.debug:
                print "saved split map in %s" % self.relative_status_directory

    def read_split_map(self):
        "Read the txt file, and replace the master Job with its corresponding JobSplit"
        split_lines = []
        map_path = self.split_map_file_path
        with open(map_path, 'r') as input_file:
            split_lines = [l.strip() for l in input_file.readlines() if l.strip()]
        if not split_lines:
            return
        master_counts = collections.Counter([l.split()[0] for l in split_lines])
        most_common = master_counts.most_common()
        max_label, max_occurrences = most_common[0]
        min_label, min_occurrences = most_common[-1]
        if max_occurrences!=1 or min_occurrences!=1:
            raise NotImplementedError("The split map %s is invalid; %d duplicates '%s'" % (map_path, max_occurrences, max_label))
        nnn = 0
        for line in split_lines:
            tokens = line.split()
            if len(tokens)<len(['master', 'child0', 'child1']):
                raise NotImplementedError("Cannot parse split map line: too few elements.\n > %s"%line)
            master_script = tokens[0]
            children_scripts = tokens[1:]
            iJob = next(i for i, job in enumerate(self.jobs) if type(job)==Job and job.script_path==master_script)
            self.jobs[iJob] = JobSplit(base_job=self.jobs[iJob],
                                       children_jobs=[Job(script_path=cs, nickname="%03d_%03d"%(nnn, mmm))
                                                      for mmm, cs in enumerate(children_scripts)],
                                       verbose=False)
            nnn += 1
        if self.debug:
            print "read %d split jobs from %s" % self.relative_status_directory

    def get_split_jobs(self): # just because 'split_jobs' is already a member function
        return [j for j in self.jobs if type(j) is JobSplit]

    @property
    def needs_to_generate_scripts(self):
        return self.overwrite_batch_scripts or not self.jobs

    def create_job(self, sample, systematic, variation, template_path=None):
        "This will need access to several specialised attributes (template, dirs, etc.)"
        raise NotImplementedError("create_job should be implemented in each specialised JobManager class")

    def generic_create_job(self, sample, systematic, variation, template_path=None):
        """create the script and append Job to self.jobs template_path
        should be used only for special samples using non-default
        template; for all other cases go through the
        implementation-specific 'create_job'.
        """
        template_path = template_path if template_path else self.template_path
        template_contents = (self.template_contents if template_path==self.template_path # use cache if default
                             else open(self.template_path).read()) # otherwise read it
        sample_name = sample.full_name.strip('/')
        # sample_name = sample.short_name # maybe this should be configurable? does the plotting/fitting code depend on it?
        is_nominal = systematic.is_nominal
        job_label = self.job_label(sample_name=sample_name, variation_name=variation.name)
        parameters = {'sample_name' : sample_name,
                      'tar_file' : self.tar_file,
                      'absolute_base_dir' : self.absolute_base_dir,
                      'relative_input_dir' : self.relative_input_directory,
                      'relative_log_dir' : self.relative_log_directory,
                      'relative_output_dir' : self.relative_output_directory,
                      'relative_status_dir' : self.relative_status_directory,
                      'filelist_name' : variation.filelist,
                      'input_tree' : variation.input_tree,
                      'output_file' : (sample_name+'.root' if is_nominal else
                                       "%s_%s.root" % (sample_name, variation.name)),
                      'dsid' : sample.dsid,
                      'compute_weight_sys' : ('true' if is_nominal else 'false'),
                      'job_label' : job_label,
                      'other_options' : '',
                      'rc_release_version' : self.rc_release_version
                      }
        batch_filename = self.relative_input_directory+'/'+sample_name+'_'+variation.name+'.sh'
        if os.path.exists(batch_filename) and not self.overwrite_batch_scripts:
            if self.debug:
                print "using existing %s" % batch_filename
        else:
            if self.debug:
                print "generating %s" % (batch_filename)
            batch_file = open(batch_filename, 'w')
            batch_file.write(template_contents.format(**parameters))
            batch_file.close()
            os.chmod(batch_filename, 0755)
            if self.verbose:
                print "created batch file %s" % batch_filename
        self.jobs.append(Job(batch_filename))

    def split_jobs(self, split_every_n=10):
        "replace large jobs with jobsplits"
        for iJob, job in enumerate(self.jobs):
            if job.number_input_files>split_every_n:
                self.jobs[iJob] = JobSplit(base_job=job, job_manager=self, max_n_input_files=split_every_n, verbose=self.verbose)

    def merge_jobs(self, min_n_input_files=10):
        "replace jobs with merged jobsets"
        jobs = []
        jobsets = []
        number_of_input_files = 0
        for job in self.jobs:
            number_of_input_files += job.number_input_files
            jobs.append(job)
            if number_of_input_files > min_n_input_files:
                jobsets.append(JobSet(script_path="%s/jobset%03d.sh"% (self.relative_input_directory, len(jobsets)),
                                      jobs=jobs,
                                      verbose=self.verbose))
                jobs = []
                number_of_input_files = 0
        if number_of_input_files:
            jobsets.append(JobSet("%s/jobset%03d.sh"% (self.relative_input_directory, len(jobsets)),
                                  jobs=jobs,
                                  verbose=self.verbose))
        self.jobs = jobsets

    def submit_jobs(self):
        submission_commands = []
        for job in self.jobs:
            if type(job) is JobSplit:
                for subjob in job.children_jobs:
                    cmd = self.job_submission_command(queue=self.queue, verbose=self.verbose,
                                                      base_dir=self.absolute_base_dir, job=subjob)
                    submission_commands.append(cmd)
            else:
                cmd = self.job_submission_command(queue=self.queue, verbose=self.verbose,
                                                  base_dir=self.absolute_base_dir, job=job)
                submission_commands.append(cmd)
        for cmd in submission_commands:
            if self.verbose:
                print cmd
            if not self.dry_run:
                out = utils.get_command_output(cmd)
                # status_path = os.path.join(self.relative_status_directory, job.nickname+'.submitted')
                # with open(status_path, 'w') as status_file:
                #     status_file.write('stdout:\n'+out['stdout']+
                #                       'stderr:\n'+ out['stderr'])
        if self.dry_run:
            print "This was a dry run. To actually submit the jobs run with '--submit'"
    def check_outputs(self):
        counter_job_any = 0
        counter_job_done = 0
        counter_job_miss = 0
        files_done = []
        files_miss = []
        for job in self.jobs:
            out_filenames = job.expected_output_files
            done = [f for f in out_filenames if os.path.exists(f)]
            miss = [f for f in out_filenames if f not in done]
            files_miss.extend(miss)
            files_done.extend(done)
            all_done = len(done)==len(miss)
            counter_job_any += 1
            counter_job_done += (1 if all_done else 0)
            counter_job_miss += (0 if all_done else 1)
        print "Checked %d jobs: %d done, %d missing" % (counter_job_any, counter_job_done, counter_job_miss)
        print "\t  (%d/%d output files)" % (len(files_done), len(files_done)+len(files_miss))
        if self.verbose:
            print 'Missing files:\n'+'\n'.join(files_miss)

    def resubmit_failed_jobs(self):
        """For merged jobs, now resubmitting the whole script (even when partially done)

        TODO implement a smarter JobSet resubmit where only the
        required 'runN' functions are called, and the others are
        commented out.
        """
        self.check_outputs()
        unfinished_jobs = []
        for job in self.jobs:
            if type(job) is JobSplit:
                unfinished_subjobs = [cj for cj in job.children_jobs if not Job.is_completed(cj)]
                unfinished_jobs.extend(unfinished_jobs)
            elif not Job.is_completed(job):
                unfinished_jobs.append(job)
        self.jobs = unfinished_jobs
        # note to self: this will not leave any partial JobSplit in the SplitMap, avoiding that it gets corrupted
        if self.verbose:
            print "about to resubmit %d failed jobs" % len(self.jobs)
        self.submit_jobs()

    @property
    def relative_input_directory(self):
        "where the job script files will be generated"
        return 'batch/'+self.jobset+'/input'
    @property
    def relative_log_directory(self):
        return 'batch/'+self.jobset+'/log'
    @property
    def relative_output_directory(self):
        return 'batch/'+self.jobset+'/output'
    @property
    def relative_status_directory(self):
        return 'batch/'+self.jobset+'/status'
    def create_directories(self, subdir='', verbose=False):
        for d in [self.relative_input_directory, self.relative_log_directory,
                  self.relative_output_directory, self.relative_status_directory]:
            dir_path = utils.mkdir_if_needed(d+(('/'+subdir) if subdir else ''), verbose=verbose)
    def job_label(self, sample_name=None, variation_name=None):
        "The label used to distinguish one job from another."
        job_label = sample_name+'_'+variation_name
        return job_label
    def default_tar_file(self):
        return base_directory()+'/'+self.relative_input_directory+'/packages.tgz'
    def check_tar_file(self, path):
        if not os.path.exists(path) or self.overwrite_tar:
            self.prepare_tar_file(tar_file_name=path)
        elif self.verbose:
            print "Using existing tar file: %s"%path

    def prepare_tar_file(self, tar_file_name=None):
        "create tar; behaves as GNU 'tar', i.e. by default it overwrites existing files"
        cmd  = "tar czf  %s " % tar_file_name
        cmd += " BtaggingTRFandRW IFAEReweightingTools IFAETopFramework VLQAnalysis"
        cmd += " --exclude='.svn' --exclude='.git' --exclude='*.o' --exclude='*.so'"
        out = utils.get_command_output(cmd)
        if out['returncode']!=0:
            print "Failed to create tar file %s" % tar_file_name
            print out['stderr']
            print out['stdout']
        elif self.verbose:
            print out['stderr']
            print out['stdout']
            print "Created tar file %s" % tar_file_name

    @property
    def tar_file(self):
        if not self._tar_file: # set value on first call (and check existence if necessary)
            self._tar_file = self.default_tar_file()
            self.check_tar_file(self._tar_file)
        return self._tar_file
    @tar_file.setter
    def tar_file(self, value):
        value = os.path.abspath(value)
        self.check_tar_file(value)
        self._tar_file = value
    def default_queue(self):
        return self.queues[0]
    @property
    def queue(self):
        if not self._queue: # set value on first call (and check existence if necessary)
            self._queue = self.default_queue()
        return self._queue
    @queue.setter
    def queue(self, value):
        if value not in self.queues:
            raise ValueError("invalid queue '%s', must be from %s" % (value, str(self.queues)))
        self._queue = value
    @property
    def template_contents(self):
        if not self._template_contents:
            self._template_contents = open(self.template_path).read()
        return self._template_contents



#___________________________________________________________

class LxbJobManager(JobManager):
    "Job manager for lxbatch queues at cern"
    def __init__(self, jobset_label, **kwargs):
        super(LxbJobManager, self).__init__(jobset_label, **kwargs)
        self.queues = ['8nm', '1nh', '8nh', '1nd', '2nd', '1nw', '2nw', 'test'] # bqueues -u ${USER}
        self.template_path = 'VLQAnalysis/data/hbsm/batch/templates/cern/one_lep_nom.sh'
    def job_submission_command(self, queue=None, verbose=None, base_dir=None, job=None):
        cmd = (" bsub "
               +" -L /bin/bash " # reset shell
               +" -q %s " % queue
               # not sure this is working
               # +" -o %s/tthf-trex-utils/batch/log/%s_%s.oe" % (base_dir, opts.label, nickname)
               +" -J %s " % job.nickname
               +" -o %s.oe " % (self.relative_log_directory+'/'+job.nickname)
               +" %s" % os.path.join(base_dir, job.script_path)
               )
        return cmd
    def create_job(self, sample, systematic, variation, template_path=None):
        self.generic_create_job(sample, systematic, variation, template_path)



#___________________________________________________________

class At3JobManager(JobManager):
    "Job manager for at3 queues at pic"
    def __init__(self, jobset, **kwargs):
        super(LxbJobManager, self).__init__(jobset_label, **kwargs)
        self.queues = ['at3_short', 'at3', 'at3_8h', 'at3_xxl']
        self.template_path = 'VLQAnalysis/data/hbsm/batch/templates/ifae/one_lep_nom.sh'
    def create_job(self, sample, systematic, variation, template_path=None):
        self.generic_create_job(sample, systematic, variation, template_path)
    def job_submission_command(self, queue=None, verbose=None, base_dir=None, job=None):
        cmd = (" qsub "
               +" -j oe " # join stdout and stderr
               +" -o %s.oe " % (self.relative_log_directory+'/'+job.nickname)
               +" -q %s " % queue
               +" %s" % os.path.join(base_dir, job.script_path)
               )
        return cmd

def guess_batch_platform():
    out = utils.get_command_output('hostname --domain')
    domain = out['stdout'].strip()
    if domain=='cern.ch':
        return 'lxbatch'
    elif domain=='pic.es':
        return 'at3'
    else:
        raise NotImplementedError("unknown domain '%s'; only pic.es and cern.ch are currently supported" % domain)

def guess_rc_release_version():
    # out = utils.get_command_output("lsetup 'rcSetup --printMyRelease'")
    # out = utils.get_command_output("rcSetup --printMyRelease", with_current_environment=True)
    # print 'release: out >>>>>>>>>>> ',out['stdout'].strip()
    # print 'release: err >>>>>>>>>>> ',out['stderr'].strip()
    # rc_release_version = out['stdout'].strip()
    # TODO the solution above does not work; for now hardcode it here (at least it is in one single place)
    rc_release_version = 'Base 2.4.14'
    return rc_release_version

#___________________________________________________________

if __name__=='__main__':
    print "Testing job manager"

    # sc_hbsm = HbsmSampleCatalogue()
    # # sc_hbsm.add_samples_from_group_files(glob.glob('VLQAnalysis/data/groups/hbsm/*.txt'))
    # sc_hbsm.add_samples_from_group_files(glob.glob('VLQAnalysis/data/groups/hbsm/hbsm.txt'))
    # sc_hbsm.samples = sc_hbsm.add_systematic_variations(sc_hbsm.samples)
    # input_from_dir = LocalDiskInterface(filelist_dir='VLQAnalysis/data/filelist',
    #                                     base_input_dir='/tmp/gerbaudo/rucio')
    # input_from_eos = RucioEosCernInterface()
    # sc_hbsm.add_filelists(samples=sc_hbsm.samples, input_interface=input_from_eos)
    # # sc_hbsm.add_filelists(samples=sc_hbsm.samples, input_interface=input_from_dir)

    # job_manager = LxbJobManager('test_2016-10-19')
    # job_manager.queue = '8nh'
    # job_manager.verbose = True
    # job_manager.dry_run = True # False
    # for sample in sc_hbsm.samples:
    #     for systematic in sample.systematic_uncertainties:
    #         for variation in [v for v in systematic.variations if v.name=='nominal']:
    #             job_manager.create_job(sample, systematic, variation)
    # job_manager.submit_jobs()

    print 'testing JobSet.extract_function'
    print JobSet.extract_function(script_path='batch/test_2016-10-24/input/user.mcasolin.341543.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a821_r7676_p2669.HtX4Tops_00-00-12_out.root_nominal.sh',
                            starting_token=Job.keyword_run_function)

    print 'testing Job.parse_run_cmd_line'
    print Job.parse_run_cmd_line('batch/test_2016-10-26b/input/user.mcasolin.341541.aMcAtNloPythia8EvtGen.DAOD_TOPQ1.e4336_a766_a821_r7676_p2669.HtX4Tops_00-00-12_out.root_nominal.sh')
