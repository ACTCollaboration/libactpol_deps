#!/usr/bin/env python

#  An early, unmaintained test of some slim behavior.  See unittest
#  for real unit testing.

#  Copyright (C) 2008, 2009 Joseph Fowler
#
#  This file is part of slim, a compression package for science data.
#
#  Slim is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Slim is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with slim.  If not, see <http://www.gnu.org/licenses/>.

import subprocess, sys, os

###########################################################################
# Control test behavior
#
# Choose a system binary that hashes arbitrary files, e.g.
# md5sum or cksum.
#
if sys.platform.startswith("linux"):
    checksum_cmd=['md5sum']
elif sys.platform == "darwin":
    checksum_cmd=['md5','-q']
else:
    raise NotImplementedError("Checksum binary not known for sys.platform='%s'"%
                              sys.platform)
#
# Change verbose to adjust amount of output
#
verbose=False
#
#
#
raw_datafile = '/tmp/fake_test_data.bin'
###########################################################################


def try_shell_command(cmd, fail_on_error = False):
    ps = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = ps.communicate()[0]

    if ps.returncode == 0:
        return output

    print "----->>>>> Error: command returned code %d. Command was:"%\
        ps.returncode
    print " ".join(cmd)
    print "----->>>>> command output is:"
    print output
    print "----->>>>> end of command output"
    print
    if fail_on_error:
        e = subprocess.CalledProcessError("Process failed")
        e.cmd = cmd
        e.returncode = ps.returncode
        e.output = output
        raise e

def setup():
    pass


def test_data_types(std_file = '/tmp/data_partial.bin'):
    '''
    Compress a file and make sure it works regardless of the data type
    '''
    global verbose

    slm_file = '%s.slm'%std_file
    test_file = '%s.raw'%std_file
    print 'Running compression data-type tests on %s (size %d)...'%(
        std_file, os.stat(std_file).st_size)
    std_sum = try_shell_command(checksum_cmd+[std_file], fail_on_error=True).split()[0]
    std_size = os.stat(std_file).st_size

    sums=[]
    sizes=[]
    base_command = ['../bin/slim','--preserve','-k', '-C']
    command_list = [base_command + dtype + [compr, std_file] for
                    compr in ('-m2','-dm2') for
                    dtype in ('-i -c1 -r16384'.split(),
                              '-u -c1 -r16384'.split(),
                              '-s -c2 -r1 -F16384'.split(),
                              '-v -c2 -r1 -F16384'.split(),
                              '--char -c2 -r2 -F16384'.split()
                    )]
    unslim_cmd = ['../bin/slim','--preserve', '--expand', '-k', slm_file]
    clean_cmd = ['rm', '-f',slm_file, test_file]
    for c in command_list:
        subprocess.call(clean_cmd)
        if verbose: print " ".join(c)
        out = try_shell_command(c)
        if verbose: print out

        if verbose: print " ".join(unslim_cmd)
        try_shell_command(unslim_cmd, fail_on_error=True)
        out = try_shell_command(checksum_cmd+[test_file])
        sums.append(out.split()[0])
        sizes.append(os.stat(test_file).st_size)

    # Check for any sum mismatches
    mismatch=0
    for sum in sums:
        if sum != std_sum:
            mismatch += 1
            verbose = True

    # Report, if verbose
    if verbose:
        print '\nFile expansion test checksums\n%s size %d (original file)'%(
            std_sum, std_size)
        for (sum,size) in zip(sums,sizes):
            if sum == std_sum:
                warning=''
            else:
                warning='*'*20
            print "%32s size %7d %s"%(sum,size,warning)

    # Summary of tests
    if mismatch:
        print '...Failed %d of %d expansion tests'%(mismatch, len(command_list))
        print "** In some tests, the slimfile did not expand to the "+\
              "correct raw file. **\n"
        raise RuntimeError('Failed tests')
    else:
        print '...Passed all %d data-type tests\n'%(len(command_list))


def test_compression_buffering(std_file = '/tmp/data_partial.bin',
                               bufsizes=[100000]):
    '''
    Compress a file and make sure the buffered versions make
    slimfiles identical with the normal file-at-a-time version.
    '''
    global verbose

    slm_file = '%s.slm'%std_file
    print 'Running compression tests on %s (size %d)...'%(
        std_file, os.stat(std_file).st_size)

    slim_cmd= ['../bin/slim', '--preserve', '-k', '-c1', '-Ci', '-r16384',
               '-dm2']
    if verbose: print " ".join(slim_cmd)

    msg = try_shell_command(slim_cmd+[std_file], fail_on_error=False)
    if verbose:
        print msg

    out = try_shell_command(checksum_cmd+[slm_file])
    std_sum = out.split()[0]
    std_size = os.stat(slm_file).st_size

    sums=[]
    sizes=[]

    for bufsize in bufsizes:
        c = slim_cmd + ['-B%d'%bufsize, std_file]
        if verbose: print " ".join(c)
        try_shell_command(c)
        out = try_shell_command(checksum_cmd+[slm_file], fail_on_error=True)
        sums.append(out.split()[0])
        sizes.append(os.stat(slm_file).st_size)

    # Check for any checksum mismatches
    mismatch=0
    for sum in sums:
        if sum != std_sum:
            mismatch += 1
            verbose = True

    # Report, if verbose
    if verbose:
        print '\nFile compression test checksums:'
        print '%32s size %7d (original file)'%(std_sum, std_size)
        for (sum,size) in zip(sums,sizes):
            if sum == std_sum:
                warning=''
            else:
                warning='*'*20
            print "%32s size %7d %s"%(sum,size,warning)

    # Summary of tests
    if mismatch:
        print '...Failed %d of %d compression tests'%(mismatch, len(bufsizes))
        print "** Some slim files made with buffers weren't "+\
              " identical to the master. **\n"
        raise RuntimeError('Failed tests')
    else:
        print '...Passed all %d compression tests\n'%(len(bufsizes))




def test_expansion(std_file = '/tmp/data_partial.bin', bufsizes=[]):
    '''
    Compress and expand a file to verify consistency.
    Potentially do this using a set of buffer sizes for buffered reading.
    '''

    global verbose
    print 'Running expansion tests on %s (size %d)...'%(
        std_file, os.stat(std_file).st_size)

    slm_file = '%s.slm'%std_file
    test_file = '%s.raw'%std_file

    slim_cmd = ['../bin/slim','--preserve','-c1','-Cik','-r16384','-dm2',
                std_file]
    msg = try_shell_command(slim_cmd)
    if verbose: print msg
    std_sum = try_shell_command(checksum_cmd+[std_file]).split()[0]

    sums=[]
    sizes=[]
    command_list = [['../bin/slim', '-pxk',slm_file]]

    for bufsize in bufsizes:
        command_list.append(['../bin/slim', '-pxk', '-B%d'%bufsize,
                             slm_file])
    for c in command_list:
        if verbose: print " ".join(c)
        out = try_shell_command(c)

        out = try_shell_command(checksum_cmd+[test_file])
        sums.append(out.split()[0])
        sizes.append(os.stat(test_file).st_size)

    # Check for any sum mismatches
    mismatch=0
    for sum in sums:
        if sum != std_sum:
            mismatch += 1
            verbose = True

    # Report, if verbose
    if verbose:

        print '\nFile expansion test checksums\n%s (original file)'%std_sum
        for (sum,size) in zip(sums,sizes):
            if sum == std_sum:
                warning=''
            else:
                warning='*'*20
            print "%32s size %7d %s"%(sum,size,warning)

    # Summary of tests
    if mismatch:
        print '...Failed %d of %d expansion tests'%(mismatch, len(command_list))
        print "** In some tests, the slimfile did not expand to the "+\
              "correct raw file. **\n"
        raise RuntimeError('Failed tests')
    else:
        print '...Passed all %d expansion tests\n'%(len(command_list))



def make_partial_file(size=4000000):
    '''
    Make a file of the given size (bytes) by copying part of a source file
    to test partial-frame encoding.
    '''
    source = raw_datafile
    dest = '/tmp/data_partial.bin'

    cmd = 'dd bs=%d if=%s of=%s count=1'%(size, source, dest)
    subprocess.call(cmd.split())
    return dest



def make_full_file(size=10000000):
    '''
    Make a random data file of the given size (bytes).
    '''
    pass



def main():
    global verbose
    setup()

    bufsizes=[2000000,
              262144,  # 4 sections
              65537,   # 1 section + 1 byte
              65536,   # 1 section
              65535,   # 1 section - 1 byte
              16384,   # 1/4 section
              70000,   # > 1 section
              60000,   # < 1 section
              30000,   # < 1/2 section
              29997,   # Odd word
              256,     # Very small
              101, 23, 5,
              4,       # One word
              3,       # < 1 word
              ]

    # Test on a file having a partial frame at the end
    # Here, one expansion buffer will suffice.
    datafile = make_partial_file(1000000)
    test_data_types(datafile)
    test_compression_buffering(datafile, bufsizes=bufsizes)
    test_expansion(datafile, bufsizes=bufsizes)

    # Test on a file having a partial word at the end
    # Here, one expansion buffer will suffice.
    datafile = make_partial_file(1000003)
    test_data_types(datafile)

    # Test on a file having an extra byte beyond last frame
    # Here, one expansion buffer will suffice.
    datafile = make_partial_file(8*65536+1)
    #verbose=True
    test_data_types(datafile)
    verbose=False

    # Test on a file one byte short of a full frame
    # Here, one expansion buffer will suffice.
    datafile = make_partial_file(8*65536-1)
    test_data_types(datafile)

    # Test on a complete 17 MB file, using full set of buffer
    # sizes on both compression and expansion
    big_file = raw_datafile
    test_data_types(big_file)
    test_compression_buffering(big_file, bufsizes=bufsizes)
    test_expansion(big_file, bufsizes=[65537])


# When called as a script, just run main.
if __name__ == '__main__': main()
