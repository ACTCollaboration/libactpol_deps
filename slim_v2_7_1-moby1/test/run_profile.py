#!/usr/bin/env python

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

import commands, os

def total_time(time_str):
    '''Parse the result of the bash 'time' command'''
    t_tuple = time_str.split()
    u,s = (t_tuple[-3], t_tuple[-1])
    min,sec = u.split('m')
    u = float(min)*60+float(sec.split('s')[0])
    min,sec = s.split('m')
    s = float(min)*60+float(sec.split('s')[0])
    #print 'Time = %.3f + %.3f = %.3f sec'%(u,s, u+s)
    return u,s

class Compressor:
    def __init__(self,compress,decompress,suffix,name=None):
        self.name=name
        if (not name): self.name=compress
        self.compress=compress
        self.decompress=decompress
        self.suffix=suffix
        self.result=None

    def __str__(self):
        return '%-20.20s %s'%(self.name, self.result)

class Performance:
    def __init__(self, timec, timed, ratio):
        self.timec = timec
        self.timed = timed
        self.ratio = ratio

    def __repr__(self):
        return 'Performance(%g,%g,%g)'%(self.timec, self.timed, self.ratio)

    def __str__(self):
        return '%7.3f %7.3f  %7.5f'%(self.timec, self.timed, self.ratio)

class RawFile:
    def __init__(self, name, nchan=1, reps=1):
        self.name = name
        self.slim_name = '%s.slm'%name
        self.fat_name = '%s.raw'%name
        self.all_names = (self.name, self.slim_name, self.fat_name)
        self.nchan=nchan
        self.reps=reps

    def __del__(self):
        '''Destructor: remove the slim and re-fattened files'''
        try:
            os.remove(self.fat_name)
            os.remove(self.slim_name)
        except OSError:
            pass




    def compress(self,C):
        compressed_name = '%s%s' % (self.name,C.suffix)
        cmd = 'time %s %s'%(C.compress,self.name)
        t1=commands.getoutput(cmd)
        #print cmd
        #print t1
        t1=total_time(t1)
        compressed_size = os.stat(compressed_name).st_size
        if compressed_size <= 0:
            print 'Oh-oh: file %s has size %d'%(compressed_name,
                                                compressed_size)
            print t1
            print C.compress,
            print self.name

        cmd = 'time %s %s'%(C.decompress,compressed_name)
        t2=commands.getoutput(cmd)
        #print cmd
        #print t2
        t2=total_time(t2)
        C.result = Performance(sum(t1),sum(t2),
                               self.raw_size/float(compressed_size))


        
    def test(self, slimargs=None):

        # First, confirm that the test file exists.
        try:
            self.raw_size=float(os.stat(self.name).st_size)
        except OSError:
            print 'File %s does not exist'%self.name
            return
        
        
        # Standard zipping programs
        if not slimargs:
            slimargs=''
        slim_program='slim --preserve -c%d -r%d %s '%(
            self.nchan, self.reps, slimargs)
        unslim_program = 'slim --preserve -x'
        RB =Compressor(slim_program+' -m1',unslim_program,
                      '.slm',name='Reduced binary')
        RBd=Compressor(slim_program+' -dm1',unslim_program,
                      '.slm',name='Reduced binary delt')
        A =Compressor(slim_program+' -m2',unslim_program,
                      '.slm',name='Code A')
        Ad=Compressor(slim_program+' -dm2',unslim_program,
                      '.slm',name='Code Adelt')
        B =Compressor(slim_program+' -m3',unslim_program,
                      '.slm',name='Code B')
        Bd=Compressor(slim_program+' -dm3',unslim_program,
                      '.slm',name='Code Bdelt')
        H =Compressor(slim_program+' -m4',unslim_program,
                      '.slm',name='Huffman')
        Hd=Compressor(slim_program+' -dm4',unslim_program,
                      '.slm',name='Huffman delt')

        CM=Compressor('compress','uncompress', '.Z')
        GZ1=Compressor('gzip --fast','gunzip', '.gz')
        GZ9=Compressor('gzip --best','gunzip', '.gz')
        BZ1=Compressor('bzip2 --fast','bunzip2', '.bz2')
        BZ9=Compressor('bzip2 --best','bunzip2', '.bz2')
 

        print '                     Compress Expand  Compression'
        print 'Program                time    time    ratio'
        print '----------            -------  -----  -------'
        #for c in (RB, A,B,H, RBd,Ad,Bd,Hd):#, CM,GZ1,GZ9,BZ1,BZ9):
        #for c in (A,B,Ad,Bd,GZ1, BZ1):#, CM,GZ1,GZ9,BZ1,BZ9):
        for c in (A,Ad):#, CM,GZ1,GZ9,BZ1,BZ9):
            self.compress(c)
            print c
            self.digest()
        print '----------            -------  -----  -------'
        print 'File name %s'%self.name
        print 'Original size %d'%self.raw_size


    def digest(self):
        '''Compute and compare a file digest of the original file
        with that of the re-expanded file.'''
        digest_cmd = 'md5sum'
        digest_output = commands.getoutput('%s %s %s'%(
            digest_cmd, self.name, self.fat_name))
        #print digest_output
        
        digest_output = digest_output.split()
        dig0 = digest_output[0]
        dig1 = digest_output[2]
        if (dig0 != dig1):
            print 'Yikes!  md5sums are %s != %s'%(dig0, dig1)
            print commands.getoutput('cmp %s %s'%(self.name, self.fat_name))
            print commands.getoutput('ls -l %s %s'%(self.name, self.fat_name))
        else:
            pass #print 'md5sums match = %s'%dig0

    # --------------------------------------------------------------
    # Don't use these 
##     def call_text(self,expand=False,deltas=False, code=1):
##         '''Return the cmdline text to perform the requested compress/
##         decompress step'''
##         file_arg = self.name
##         expand_text = delta_text = ''
##         if (deltas): delta_text='-d'
##         if (expand):
##             expand_text='-x'
##             file_arg = self.slim_name
##         code_text = '-e%d'%code
        
##         str='time ./slim %s -n%d %s %s %s > /dev/null'%(
##             code_text,self.nchan,expand_text,delta_text,file_arg)
##         return str


##     def test_one_slim(self, deltas=False, code=1):
##         t1 = commands.getoutput(self.call_text(deltas=deltas, code=code))
##         total_time(t1)
##         t2 = commands.getoutput(self.call_text(expand=True))
##         total_time(t2)

##         sizes = (
##             os.stat(self.name     ).st_size,
##             os.stat(self.slim_name).st_size,
##             os.stat(self.fat_name ).st_size,
##             )
##         ratio = float(sizes[0])/sizes[1]
##         #print 'Compression ratio = ', ratio
##         self.digest()
    # --------------------------------------------------------------
                                        
        

def test1():
    commands.getoutput('make slim')
    #commands.getoutput('cat DATA/tesdatar??c?? > DATA/tescat')

##     #RawFile('hk_fake2.dat',33,reps=125000).test()
##     RawFile('hk_fake.dat',33,reps=125000).test()
##     RawFile('/data/mce/1173484228', 108).test()
##     RawFile('/data/mce/1173482145', 108).test()
##     RawFile('/data/mce/1172818941', 308).test()
##     RawFile('/data/mce/1173246883', 308).test()


    RawFile('/data/mce/9sec_1196340620_dat',1100,1).test('-i -F3600')
    RawFile('DATA/tescat', 1, 16384).test('-i')
    RawFile('DATA/thermom', 1, 504084).test('-i -b')
    #RawFile('/data/mce/1196340620_dat',1100,1).test('-i -F3600')

if __name__ == '__main__': test1()
