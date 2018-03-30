/// \file ljhslim_control.cpp
/// Implements to slim_control class to parse command-line arguments.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <utime.h>

#include <slim.h>
#include "ljhslim.h"

#define RAW_SUFFIX ".raw"   ///< Suffix for all raw files when preserving original.


/// Single-character options
static const char short_opt[] = "dnpSC0bPok987654321V?B:";

/// The long options slim recognizes.
static struct option long_opt[] = {
  {"deltas",            no_argument,       NULL, 'd'},
  {"filename",          no_argument,       NULL, 'n'},
  {"preserve",          no_argument,       NULL, 'p'},
  {"rawsize",           no_argument,       NULL, 'S'},
  {"compute-crc32",     no_argument,       NULL, 'C'},
  {"ignore-crc32",      no_argument,       NULL, '0'},
  {"permit-bitrotation",no_argument,       NULL, 'b'},
  {"practice",          no_argument,       NULL, 'P'},
  {"stdout",            no_argument,       NULL, 'o'},
  {"force",             no_argument,       NULL, 'k'},
  {"best",              no_argument,       NULL, '9'},
  {"18-pct",            no_argument,       NULL, '9'},
  {"16-pct",            no_argument,       NULL, '8'},
  {"14-pct",            no_argument,       NULL, '7'},
  {"12-pct",            no_argument,       NULL, '6'},
  {"10-pct",            no_argument,       NULL, '5'},
  {"8-pct",             no_argument,       NULL, '4'},
  {"6-pct",             no_argument,       NULL, '3'},
  {"4-pct",             no_argument,       NULL, '2'},
  {"2-pct",             no_argument,       NULL, '1'},
  {"fast",              no_argument,       NULL, '1'},
  {"version",           no_argument,       NULL, 'V'},
  {"help",              no_argument,       NULL, '?'},
  {"debug-buffer",      required_argument, NULL, 'B'},
  {0, 0, 0, 0}
};

// ---------------------------------------------------------------------------
/// \class ljhslim_control
/// Handles command-line arguments and keeps track of control settings.
// ---------------------------------------------------------------------------
///


/// Constructor.
ljhslim_control::ljhslim_control() {
  set_defaults();
}





/// Parse command-line arguments.
/// \param argc Number of tokens in command line. 
/// \param argv Array of ptr to these tokens.
void ljhslim_control::process_args(int argc, char * const argv[]) 
{
  bool print_version=false, print_help=false;

  for (opterr = 0;;) {
    char c;
    int opt_index;
    if ((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) < 0)
      break;
    switch (c) {
    case 0:
      if (!strncmp(long_opt[opt_index].name, "help",
		   strlen(long_opt[opt_index].name)))
	usage();
      break;
      
    case 'o':
      slimcat();
      break;

    case 'B':
      debug_buf_size = atoi(optarg);
      break;

    case 'd':
      deltas = true;
      break;

    case 'n':
      save_filename = true;
      break;

    case 'p':
      preserve_input = true;
      break;

    case 'S':
      save_rawsize = true;
      break;

    case 'V':
      print_version = true;
      break;

    case 'C':
      crc = true;
      break;

    case '0':
      ignore_crc = true;
      break;

    case 'P':
      practice=true;
      break;

    case 'k':
      force_clobber = true;
      break;

    case 'b':
      permit_bitrotation= true;
      break;

    case '9': case '8': case '7':
    case '6': case '5': case '4':
    case '3': case '2': case '1':
      sample_pct = 2*(c-'0');
      assert (sample_pct >= 1 && sample_pct <= 100);
      break;

    case '?':
      print_help = true;
      break;

    default:
      throw "Error processing options with getopt_long:"
        " unrecognized return value";
      break;
    }
  }

  // Version or help messages are printed, then exit.
  if (print_version)
    version();
  if (print_help)
    usage();
  if (print_version || print_help)
    exit(0);

  if (practice) {
    mode = SLIM_ENCODE;
    preserve_input = true;
  }
}




/// Print a usage message.
void ljhslim_control::usage() const {
  cout << "usage: ljhslim    [options] filename ...\n";
  cout << "Allowed options are:\n";
  
  usage_printoptions();
}


/// Print the options portion of a usage message.
void ljhslim_control::usage_printoptions() const {
  const char noarg[]="", 
    hasarg[]="<arg required>", 
    optional[]="[arg optional]";
  const char *argval[3]={noarg, hasarg, optional};

  ios_base::fmtflags old = cout.setf(ios_base::floatfield);
  cout.setf(ios_base::left);
  for (struct option *opt = long_opt; opt->name; opt++) {
    cout << "-" << char(opt->val) << ", --";
    cout.width(20);
    cout << opt->name << " " << argval[opt->has_arg] << "\n";
  }
  cout.setf(old);
}



/// Print version message.
void ljhslim_control::version() const 
{
  cout << "This is ljhslim, the data compressor for LJH calorimeter files.\n";
}



/// Set members to default values.
void ljhslim_control::set_defaults()
{
  deltas = false;
  repeats = 0;
  debug_buf_size = 0;
  sample_pct = 10;
  mode = SLIM_ENCODE;
  code_method = SLIM_ENCODER_REDUCED_BINARY;
  force_clobber = false;
  preserve_input = false;
  permit_bitrotation = false;
  use_stdout = false;

  save_rawsize = true;
  save_filename = false;
  have_xtra = false;
  have_toc = false;
  onechan = false;
  noreps = false;
  crc = false;
  ignore_crc = false;
  reserved0 = false;
  practice = false;
}



/// Handle one file, by encoding or decoding it.
/// \param fname  The name of the file to be encoded or decoded.
void ljhslim_control::handle_one_file(const char *fname)
{
  enum slim_mode_t thismode = mode;

  if (thismode == SLIM_MODE_UNKNOWN)
    thismode = detect_file_mode(fname);
  assert(thismode == SLIM_ENCODE || thismode == SLIM_DECODE);

  // Make sure the file exists and doesn't have multiple hard links
  struct stat st;
  int ret = stat(fname, &st);
  if (ret)
    throw bad_file(fname, ": does not exist.");
  if (S_ISDIR(st.st_mode)) {
    throw bad_file(fname, " is a directory -- ignored.");
  }
  if (!S_ISREG(st.st_mode)) {
    throw bad_file(fname, ": not a regular file.");
  }
  if (st.st_nlink > 1 && !force_clobber && !preserve_input) {
    throw bad_file(fname, ": has more than one hard link (use -k to force).");
  }

  // Make sure that we have read permission on the input file
  if (access(fname, R_OK)) {
    if (errno == EACCES)
      throw bad_file(fname, ": read permission denied.");
    else
      throw bad_file(fname, ": access() call failed; not with EACCES error.");
  }

  if (thismode == SLIM_ENCODE) {
    if (debug_buf_size == 0)
      compress_one_file(fname);
    else {
      debug_compress_from_memory(fname);
    }
  } 

  if (thismode == SLIM_DECODE) {
    if (debug_buf_size == 0)
      expand_one_file(fname);
    else
      debug_expand_from_memory(fname);
  }
}



/// Find the length of the text header in the ljh file
static void parse_ljh_header(const char *rawname,
                                   size_t &size,
                                   int& nsamples) {
  const string ENDHEADER("#End of Header");
  const size_t LINESIZE=512;
  char line[LINESIZE];
  line[LINESIZE-1] = '\0';

  nsamples=0;
  size=0;
  ifstream fp(rawname);

  // Loop over lines until one starts with the ENDHEADER text.
  while (1) {
    fp.getline(line, LINESIZE-1);

    // Found end-of-file before ENDHEADER?  If so, this must not be an LJH file
    if (fp.eof())
      throw bad_file(rawname, ": is not an LJH file. No #End of Header marker");

    // Found line longer than LINSIZE?  If so, this must not be an LJH file
    if (fp.fail())
      throw bad_file(rawname, ": is not an LJH file. Long line found");

    string sline(line);
    size += sline.size() + 1; // +1 because fp.getline strips the \n character

    for (int i=0; i<sline.size(); i++) {
      if (sline[i] < '\t' || sline[i] >= 0x7f )
        throw bad_file(rawname, ": is not an LJH file. Non-ASCII char in header");
    }

    if (sline.find("Total Samples:") == 0) {
      size_t pos = sline.find_first_of("0123456789");
      istringstream xxx(line+pos);
      xxx >> nsamples;
    }
    if (sline.find(ENDHEADER)==0) break;

  }
  fp.close();
}


/// Alter the access and modification time on a closed file.
/// \param filename  The file to alter.
/// \param mtime     The new value for filename's atime and mtime.
/// \return          Returns 0 on success or -1 on error (and errno is set).
inline int alter_mtime(const char *filename, time_t mtime) {
  struct utimbuf timbuf;
  timbuf.actime = timbuf.modtime = mtime;
  return utime(filename, &timbuf);
}



/// Compress one file using slim_compressor_t::compress_from_file()
/// \param rawname  The name of the file to be encoded.
void ljhslim_control::compress_one_file(const char *rawname) {
  
  // Verify that file exists
  struct stat st;
  int ret = stat(rawname, &st);
  if (ret)
    throw bad_file(rawname, ": does not exist.");
  const size_t raw_file_size = st.st_size;

  cout.width(20);
  cout << rawname << ": ";

  // Create the output slim-file name, create compressor object.
  ostringstream outstr;
  outstr << rawname << '.' << SLIM_SUFFIX;
  string outname_string = outstr.str();
  const char *outname = outname_string.c_str();

  if (!force_clobber) {
    ret = stat(outname, &st);
    if (ret == 0)
      throw bad_file(outname, ": slim file exists (use -k to force).");
  }

  int nsamples=0;
  size_t nbytes_text = 0;
  parse_ljh_header(rawname, nbytes_text, nsamples);

  slim_compressor_t *compressor =
    new slim_compressor_t(outname, flags(), deltas, sample_pct);
  compressor->get_input_file_stats(rawname);
  
  FILE *fp = fopen(rawname, "rb");

  // Write the header
  int repeats_header = nbytes_text;
  int nframes_header = 1;
  compressor->add_channel(repeats_header, code_method,
                          SLIM_TYPE_U8, false, false);

  unsigned char *header = new unsigned char[nbytes_text];
  size_t count = fread(header, 1, nbytes_text, fp);
  assert(count == nbytes_text);
  compressor->write_onesection(header, nbytes_text);
  delete[] header;

  // Set up to write the binary data that follows the header.
  compressor->reset_channels();
  const int repeats_dummychan = 1;
  const int repeats_timestamp = 1;
  const int repeats_thisfile = nsamples;
  compressor->add_channel(repeats_dummychan, code_method, 
                          SLIM_TYPE_U16, deltas, permit_bitrotation);
  compressor->add_channel(repeats_timestamp, code_method, 
                          SLIM_TYPE_U32, deltas, permit_bitrotation);
  compressor->add_channel(repeats_thisfile, code_method, 
                          SLIM_TYPE_U16, deltas, permit_bitrotation);

  size_t frame_size = compressor->get_frame_size();
  int frames_per_sect = MAX_SECTION_LENGTH / frame_size;
  const size_t section_size = frames_per_sect * frame_size;
  unsigned char *rawdata = new unsigned char[MAX_SECTION_LENGTH];
  compressor->set_section_frames(frames_per_sect);
  while (true) {
    size_t count = fread(rawdata, 1, section_size, fp);
    if (count == 0) break;
    compressor->write_onesection(rawdata, count);
    if (count < section_size) break;
  }
  compressor->close_output();
  delete compressor;
  delete[] rawdata;
  fclose(fp);
 
  // Mark the compressed file with the raw file's modification time.
  alter_mtime(outname, st.st_mtime);
  
  // Report on the compression ratio
  {
    struct stat status;
    stat(outname, &status);
    size_t final_size = status.st_size;

    double ratio = double(raw_file_size) / final_size;
    double bitsperword = 32.0/ratio;
    double saved = 100.*(1.-double(final_size)/raw_file_size);
    cout.width(20);

    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.width(8);
    cout.precision(3);
    cout << ratio << ":1, ";
    cout.width(6);
    cout.precision(3);
    cout << bitsperword << " bits/word, ";
    cout.width(6);
    cout.precision(2);
    cout << saved << "% saved.\n";
    cout.precision(6);
  }

  // Clean up unwanted files
  struct stat st_raw, st_out;
  int ret1 = stat(rawname, &st_raw);
  int ret2 = stat(outname, &st_out);
  if (ret1 || ret2) return;
  if (!force_clobber && (st_raw.st_size <= st_out.st_size)) {
    cerr << "slim: " << rawname << 
      " expanded when slimmed (use -k to force).\n";
    unlink(outname);
  } else if (practice) {
    unlink(outname);
  } else if (!preserve_input)
    unlink(rawname);
    
}
