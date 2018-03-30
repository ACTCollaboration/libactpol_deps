/// \file slimlib.cpp
/// The C-linkable sliblib library that wraps the underlying C++ classes.

//  Copyright (C) 2008, 2009 Joseph Fowler
//
//  This file is part of slim, a compression package for science data.
//
//  Slim is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Slim is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with slim.  If not, see <http://www.gnu.org/licenses/>.


#include <string.h>
#include <unistd.h>
#include "slimlib.h"
#include "slim.h"


/// Select the read/write mode.
enum slimlib_mode_t {
  SLIMLIB_READ,     ///< This SLIMFILE object is for reading slim data.
  SLIMLIB_WRITE,    ///< This SLIMFILE object is for writing slim data.
};



typedef struct slimlib_file_t SLIMFILE;  ///< The external name is SLIMFILE.

/// All important elements of the SLIMFILE.
/// This is an opaque type from the C point of view, so
/// its contents are defined here, rather than in the header.

struct slimlib_file_t {
  enum slimlib_mode_t mode;      ///< Mode (SLIMLIB_READ or SLIMLIB_WRITE)
  struct slim_compressor_t *compressor; ///< Compressor, for _WRITE mode.
  struct slim_expander_t   *expander;   ///< Expander, for _READ mode.
  int writing_is_underway;       ///< If in _WRITE mode, did we start writing?
  int nchan;                     ///< How many channels are known?
  int nframes;                   ///< How many frames expected per section.
  long bytes_used;               ///< How many bytes read/written so far.

  /// Remember the filename or file descriptor that was opened.  
  /// We'll need this again in case user chooses to rewind.
  int fd;
  char *filename;
};


/// The following symbols need to be linkable from C.
extern "C" {



/// Open a SLIMFILE for reading or writing.
/// But as of October 2009, writing is NOT YET IMPLEMENTED!
/// \param filename  The file to open.
/// \param modes     Modes codes as in fopen.  Only "r" or "w" are used.
///                  At present, only "r" is allowed.
/// \return The open SLIMFILE, or NULL if opening fails.

SLIMFILE *slimopen(const char *filename,
		   const char *modes) {
  SLIMFILE *sf = new SLIMFILE();

  sf->compressor = NULL;
  sf->expander = NULL;
  sf->writing_is_underway = 0;
  sf->nchan = 0;
  sf->nframes = 0;
  sf->bytes_used = 0;
  sf->fd = -1;
  sf->filename = new char[1+strlen(filename)];
  strcpy(sf->filename, filename);

  try {
    if (strchr(modes, 'r')) {
      sf->mode = SLIMLIB_READ;
      sf->expander = new slim_expander_t(filename);
      if ( ! (sf->expander->is_open()) ) {
        slimclose(sf);
        return NULL;
      }

    } else if (strchr(modes, 'w')) {
      sf->mode = SLIMLIB_WRITE;
      //sf->compressor = new slim_compressor_t();
      throw "slimlib write mode has not been written yet.";
      
    } else
      throw "slimopen mode must contain 'w' or 'r'";
  } catch (const char *e) {
    slimclose(sf);
    return NULL;
  }

  return sf;
}

/// Associate SLIMFILE for reading or writing with an existing file descriptor
/// But as of October 2009, writing is NOT YET IMPLEMENTED!
/// \param fd        The descriptor of the open file.
/// \param modes     Modes codes as in fopen.  Only "r" or "w" are used.
///                  At present, only "r" is allowed.  Must be compatible with
///                  the mode of the file descriptor.
/// \return The open SLIMFILE, or NULL if opening fails.

SLIMFILE *slimdopen(int fd, const char *modes) {
  SLIMFILE *sf = new SLIMFILE();

  sf->compressor = NULL;
  sf->expander = NULL;
  sf->writing_is_underway = 0;
  sf->nchan = 0;
  sf->nframes = 0;
  sf->bytes_used = 0;
  sf->filename = NULL;

  try {
    /// This must be a duplicate because slim_expander_t will steal the fd.
    sf->fd = dup(fd);
    if (sf->fd < 0) {
      throw "unable to duplicate file descriptor.";
    }

    if (strchr(modes, 'r')) {
      sf->mode = SLIMLIB_READ;
      sf->expander = new slim_expander_t(fd);
      if ( ! (sf->expander->is_open()) ) {
        slimclose(sf);
        return NULL;
      }

    } else if (strchr(modes, 'w')) {
      sf->mode = SLIMLIB_WRITE;
      //sf->compressor = new slim_compressor_t();
      throw "slimlib write mode has not been written yet.";
      
    } else
      throw "slimopen mode must contain 'w' or 'r'";
  } catch (const char *e) {
    fprintf(stderr, "slimopen failed\n");
    slimclose(sf);
    return NULL;
  }

  return sf;
}




/// Close an open SLIMFILE.
/// \param sf  The open SLIMFILE.

int slimclose(SLIMFILE *sf) {
  try {
    if (sf->expander)
      delete sf->expander;
    if (sf->compressor)
      delete sf->compressor;
    delete [] sf->filename;
    if (sf->fd >= 0)
      close(sf->fd);
    delete sf;
  } catch (const char *e) {
    return EOF;
  }
  return 0;
}



/// Return the current position in the file (bytes from the start).
/// \param sf  The open SLIMFILE.

long slimtell(SLIMFILE *sf) {
  return sf->bytes_used;
}



/// Read bytes from open SLIMFILE into a buffer.
/// \param ptr   Buffer to be filled by data from file.
/// \param size  Size of each data word (in bytes).
/// \param nmemb Number of data words to read.
/// \param sf    Open SLIMFILE to read from.
/// \return Number of words actually read.  This will be 0 if slim raises an error.

size_t slimread(void *ptr, size_t size, size_t nmemb, SLIMFILE *sf) {
  try {
    size_t bytes = sf->expander->read((unsigned char *)ptr, size*nmemb);
    sf->bytes_used += bytes;
    return bytes / size;

    // C-linked programs can't handle (and probably don't want to know) about errors.
    // If there are read errors, simply return, noting a failure to read any words.
  } catch (char const *s) {
    return 0;
  }
}



/// Rewind the current position pointer in the file to the beginning.
/// Implemented by closing the file and starting again.
/// \param sf  The open SLIMFILE.

void slimrewind(SLIMFILE *sf) {
  sf->bytes_used = 0;
  if (sf->mode == SLIMLIB_READ) {
    delete sf->expander;
    if (sf->filename)
      sf->expander = new slim_expander_t(sf->filename);
    else
      sf->expander = new slim_expander_t(dup(sf->fd));
  } else
    throw "slimlib write not written yet";
}



/// Seek to a point in the file (analogue of fseek, which see for meaning of offset and whence).
/// \param sf  The open SLIMFILE.
/// \param offset  Byte requested, relative to beginning, current point, or end.
/// \param whence  May be SEEK_SET, SEEK_CUR, or SEEK_END.
/// \return 0 on success, -1 on error.

int slimseek(SLIMFILE *sf, long offset, int whence) {

  unsigned long uoffset = offset;
  switch (whence) {
  case SEEK_SET:
    slimrewind(sf);
    if (uoffset ==  sf->expander->read(NULL, uoffset)) 
      return 0;
    else
      return -1;
    break;

  case SEEK_CUR:
    if (offset >= 0) {
      if (uoffset ==  sf->expander->read(NULL, uoffset))
        return 0;
      else
        return -1;
    } else
      return slimseek(sf, offset + sf->bytes_used, SEEK_SET);
    break;

  case SEEK_END:  default:
    if (sf->filename == NULL)
      return slimseek(sf, offset + slimdrawsize(dup(sf->fd)), SEEK_SET);
    return slimseek(sf, offset + slimrawsize(sf->filename), SEEK_SET);
  }

}



/// Return the raw size of a compressed slim file.
/// Must open the file and read its header.
/// \param filename  The file to test by reading its header.

long slimrawsize(const char *filename) 
{
  long size;
  slim_expander_t *expander;
  expander = new slim_expander_t(filename);

  if ( expander->is_open() ) {
    size = long(expander->get_rawsize());
  } else {
    size = -1;
  }
  delete expander;
  return size;
}

long slimdrawsize(int fd)
{
  long size;
  slim_expander_t *expander;
  expander = new slim_expander_t(fd);

  if ( expander->is_open() ) {
    size = long(expander->get_rawsize());
  } else {
    size = -1;
  }
  delete expander;

  return size;
}



} // ends extern "C"
