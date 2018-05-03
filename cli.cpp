// -----------------------------------------------------------------------------
// cli.cpp                                                               cli.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref cli class.
 * @author     Col. Walter E. Kurtz
 * @version    2017-07-18
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <unistd.h>
#include <sstream>
#include "message.h"
#include "cli.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ---
// cli
// ---
/*
 *
 */
cli::cli()
{
  // set inital state
  reset();
}


// -----------------------------------------------------------------------------
// Handling                                                             Handling
// -----------------------------------------------------------------------------

// -----
// parse
// -----
/*
 *
 */
bool cli::parse(int argc, char** argv)
{
  // back to inital state
  reset();

  // this pointer must not be NULL
  if (argv == 0) return false;

  // this pointer must not be NULL
  if (argv[0] == 0) return false;

  // if the option takes an argument, optarg points to the option-argument
  extern char* optarg;

  // the index of the next element of the argv[] vector to be processed
  extern int optind;

  // optopt points to the option character that caused the error
  extern int optopt;

  // don't print error messages
  opterr = 0;

  // set executable's name
  m_argv0 = argv[0];

  // set valid option characters
  const char* optstring = ":hvbn:";

  // the ASCII code of the current option character
  int optchar;

  // parse all given options
  while ((optchar = getopt(argc, argv, optstring)) != -1)
  {
    // use this object to convert arguments
    stringstream argstream((optarg == 0) ? "" : optarg);

    // analyze (short) options
    switch (optchar)
    {
      case 'h':

        operation = SHOW_HELP;

        // next case
        break;

      case 'v':

        operation = SHOW_VERSION;

        // next case
        break;

      case 'b':

        output = BINARY;

        // next case
        break;

      case 'n':

        name = argstream.str();

        // next case
        break;

      case ':':

        // notify user
        msg::err( msg::cat("missing argument: -", int2char(optopt)) );

        // signalize trouble
        return false;

      case '?':

        // notify user
        msg::err( msg::cat("unknown option: -", int2char(optopt)) );

        // signalize trouble
        return false;
    }
  }

  // set all remaining positional parameters
  for(int i = optind; i < argc; i++)
  {
    pparams.push_back( argv[i] );
  }

  // signalize success
  return true;
}

// --------
// getXName
// --------
/*
 *
 */
string cli::getXName(bool brief) const
{
  // drop directories
  if (brief)
  {
    // filename without leading directories
    string nodir;

    // remove all directories
    for(string::size_type i = 0; i < m_argv0.size(); i++)
    {
      // check character
      if (m_argv0[i] == '/')
      {
        // reset
        nodir = "";
      }

      else
      {
        // append character
        nodir += m_argv0[i];
      }
    }

    // return brief name
    return nodir;
  }

  // return full name
  return m_argv0;
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// -----
// reset
// -----
/*
 *
 */
void cli::reset()
{
  // set default operation
  operation = DEFAULT;

  // set default type of output
  output = ASCII;

  // set default name of the stream object
  name = "cout";

  // drop positional parameters
  pparams.clear();

  // reset executable's name
  m_argv0 = "";
}

// --------
// int2char
// --------
/*
 *
 */
string cli::int2char(int ascii) const
{
  switch (ascii)
  {
    // digits
    case '0': return "0";
    case '1': return "1";
    case '2': return "2";
    case '3': return "3";
    case '4': return "4";
    case '5': return "5";
    case '6': return "6";
    case '7': return "7";
    case '8': return "8";
    case '9': return "9";

    // capital letters
    case 'A': return "A";
    case 'B': return "B";
    case 'C': return "C";
    case 'D': return "D";
    case 'E': return "E";
    case 'F': return "F";
    case 'G': return "G";
    case 'H': return "H";
    case 'I': return "I";
    case 'J': return "J";
    case 'K': return "K";
    case 'L': return "L";
    case 'M': return "M";
    case 'N': return "N";
    case 'O': return "O";
    case 'P': return "P";
    case 'Q': return "Q";
    case 'R': return "R";
    case 'S': return "S";
    case 'T': return "T";
    case 'U': return "U";
    case 'V': return "V";
    case 'W': return "W";
    case 'X': return "X";
    case 'Y': return "Y";
    case 'Z': return "Z";

    // small letters
    case 'a': return "a";
    case 'b': return "b";
    case 'c': return "c";
    case 'd': return "d";
    case 'e': return "e";
    case 'f': return "f";
    case 'g': return "g";
    case 'h': return "h";
    case 'i': return "i";
    case 'j': return "j";
    case 'k': return "k";
    case 'l': return "l";
    case 'm': return "m";
    case 'n': return "n";
    case 'o': return "o";
    case 'p': return "p";
    case 'q': return "q";
    case 'r': return "r";
    case 's': return "s";
    case 't': return "t";
    case 'u': return "u";
    case 'v': return "v";
    case 'w': return "w";
    case 'x': return "x";
    case 'y': return "y";
    case 'z': return "z";
  }

  // invalid character
  return "";
}

