#!/usr/bin/env python

"""
A random selection of unit tests that slim ought to pass, but beware
that they are largely based on new bugs later fixed and are NOT
comprehensive at the time of their writing (Feb 3, 2009).

Currently we check:

* Proper error message, non-zero return status when try to unslim 
  a non-slim file.
* Proper error messages and non-zero return status when filesystem
  problems arise, such as a file does not exist, read/write permission
  failures.

Joe Fowler, Princeton
"""

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


import unittest, numpy, glob, shutil, filecmp
import commands, os, os.path, tempfile

class test_slim(unittest.TestCase):
    """Test the proper operation of slim and related binaries."""

    def test_notSlimFile(self):
        """Should return error and report when unslim on a non-slim file."""
        fd,file = tempfile.mkstemp()
        os.close(fd)
        fp = open(file,"w")
        fp.write("random text file, not a slim file")
        fp.close()
        cmd = "unslim %s"%file
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( "not a slim file" in out )
        os.remove(file)

    def test_noSuchFile(self):
        """Should return error code and report when a file does not exist."""
        file = "ridiculousfile.dat"
        assert not os.path.exists( file )
        cmd = "slim %s"%file
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( "does not exist" in out )
        cmd = "unslim %s"%file
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( "does not exist" in out )

    def test_permissionMessages(self):
        """Should return error and report when file permissions fail."""
        # Prepare a dir, file, and slimmed version.  
        dir = tempfile.mkdtemp()
        file = "%s/text"%dir
        fp = open(file,"w")
        fp.write("random text file, not a slim file")
        fp.close()
        cmd = "slim --preserve --force -u1 -m2 %s && chmod a-r %s/*"%(file,dir)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )
        # Test
        EXPECTED_MSG = "permission denied"
        cmd = "unslim --preserve %s.slm"%file
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( EXPECTED_MSG in out )
        os.remove("%s.slm"%file)
        cmd = "slim --preserve -u1 -m2 %s"%file
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( EXPECTED_MSG in out )
        os.chmod(file, 0664)
        os.chmod(dir, 0555)
        stat,out = commands.getstatusoutput( cmd )
        self.assertNotEquals( stat, 0 )
        self.assert_( EXPECTED_MSG in out )
        # Clean up
        os.chmod(dir, 0755)
        os.remove(file)
        os.rmdir(dir)

    def test_runlengthToAlternateDeltas(self):
        """Should slim nearly-regular files, like IV sync_box_num, with --deltas"""
        self.__runlengthToAlternate(deltas=True)

    def test_runlengthToAlternateNormal(self):
        """Should slim nearly-regular files, like IV sync_box_num without --deltas"""
        self.__runlengthToAlternate(deltas=False)

    def __runlengthToAlternate(self, deltas=True):
        """Should slim regular and nearly-regular files, like IV sync_box_num"""
        N=1000
        simple = 123456+ 40*numpy.arange(N)
        harder = numpy.array(simple + numpy.random.uniform(-4,4,N), dtype=numpy.int)

        # Prepare a dir, file, and slimmed version.  
        dir = tempfile.mkdtemp()
        file1 = "%s/simple"%dir
        file2 = "%s/harder"%dir
        simple.tofile(file1)
        harder.tofile(file2)

        # Slim with or without --deltas
        if deltas:
            cmd = "slim -pC -m5 --deltas %s %s"%(file1, file2)
        else:
            cmd = "slim -pC -m5 %s %s"%(file1, file2)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )
        #print commands.getoutput("slim_dump %s/*.slm"%dir)
        
        cmd = "unslim -p %s.slm %s.slm"%(file1, file2)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        # Compare original and raw versions
        cmd = "cmp %s %s.raw && cmp %s %s.raw"%(file1, file1, file2, file2)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        # Clean up
        files = glob.glob("%s/*"%dir)
        for f in files:
            os.remove(f)
        os.rmdir(dir)

    def test_redbinToAlternateDeltas(self):
        """Should slim high-variance reduced binary files with --deltas"""
        self.__redbinToAlternate(deltas=True)

    def test_redbinToAlternateNormal(self):
        """Should slim high-variance reduced binary files without --deltas"""
        self.__redbinToAlternate(deltas=False)

    def __redbinToAlternate(self, deltas):
        """Should slim regular and nearly-regular files, like IV sync_box_num"""
        N=1000
        
        usuvar = numpy.array(numpy.random.uniform(-2e7,2e7,N), dtype=numpy.int)
        bigvar = numpy.array(numpy.random.uniform(-2e9,2e9,N), dtype=numpy.int)


        # Prepare a dir, file, and slimmed version.  
        dir = tempfile.mkdtemp()
        file1 = "%s/usuvar"%dir
        file2 = "%s/bigvar"%dir
        usuvar.tofile(file1)
        bigvar.tofile(file2)

        # Slim with or without --deltas
        if deltas:
            cmd = "slim --force -pC -m2 --deltas %s %s"%(file1, file2)
        else:
            cmd = "slim --force -pC -m2 %s %s"%(file1, file2)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )
        #print commands.getoutput("ls -l %s; slim_dump %s/*.slm"%(dir,dir))
        
        cmd = "unslim -p %s.slm"%file1
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        cmd = "unslim -p %s.slm"%file2
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        # Compare original and raw versions
        cmd = "cmp %s %s.raw && cmp %s %s.raw"%(file1, file1, file2, file2)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        # Clean up
        files = glob.glob("%s/*"%dir)
        for f in files:
            os.remove(f)
        os.rmdir(dir)


    def test_variousSectionSizesDeltas(self):
        """Should slim a 2^17 length file with various section sizes with --deltas """
        self.__variousSectionSizes(deltas=True)

    def test_variousSectionSizes(self):
        """Should slim a 2^17 length file with various section sizes without --deltas """
        self.__variousSectionSizes(deltas=False)

    def __variousSectionSizes(self, deltas=True):
        """Should slim a 2^17 length file with various section sizes"""
        N=131072
        section_sizes=(N,N/2,N/3,N/4,N/16,N/19,N/64)

        data = numpy.array(numpy.random.uniform(-2e7,2e7,N), dtype=numpy.int)

        # Prepare a dir, file, and slimmed version.  
        dir = tempfile.mkdtemp()
        file1 = "%s/usuvar"%dir
        data.tofile(file1)

        for ss in section_sizes:
            # Slim with or without --deltas
            if deltas:
                cmd = "slim --force -pC -m2 -r%d -F1 --deltas %s"%(ss,file1)
            else:
                cmd = "slim --force -pC -m2 -r%d -F1 %s"%(ss,file1)
            stat,out = commands.getstatusoutput( cmd )
            self.assertEquals( stat, 0 )
            
            cmd = "unslim --force -p %s.slm"%file1
            stat,out = commands.getstatusoutput( cmd )
            self.assertEquals( stat, 0 )

            # Compare original and raw versions
            cmd = "cmp %s %s.raw"%(file1, file1)
            stat,out = commands.getstatusoutput( cmd )
            self.assertEquals( stat, 0 )

        # Clean up
        files = glob.glob("%s/*"%dir)
        for f in files:
            os.remove(f)
        os.rmdir(dir)
        
    def test_rawFileNameStringError(self):
        """Files made by "unslim --preserve XX.slm Y.slm" should be XX.raw and Y.raw.
        Specifically, verify that we fixed a bug (issue #8) whereby unslimming
        a sequence of filenames produced bugs:
        > unslim --preserve abcde.slm wxyz.slm
        produced abcde.raw and wxyz.raww"""
        dir = tempfile.mkdtemp()
        files = ["%s/%s.slm"%(dir,fname) for fname in ("longestname","medium","short")]
        
        data = numpy.array(numpy.random.uniform(-10000,10000,8192), dtype=numpy.int32)
        rawfile = "%s/rawdata"%dir
        slimfile = rawfile+".slm"
        data.tofile(rawfile)
        cmd = "slim --force -pC -m2 -F1 -r8192 %s"%rawfile
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )
        for f in files:
            shutil.copy(slimfile, f)
        cmd = "unslim --preserve " + " ".join(files)
        stat,out = commands.getstatusoutput( cmd )
        self.assertEquals( stat, 0 )

        for f in files:
            expected_raw = f[:-3]+"raw"
            self.assertTrue( os.path.isfile(expected_raw) )
            self.assertTrue( filecmp.cmp(rawfile, expected_raw) )

        tmpfiles = glob.glob("%s/*"%dir)
        for file in tmpfiles:
            os.remove(file)
        os.rmdir(dir)

if __name__ == "__main__": 
    unittest.main()
