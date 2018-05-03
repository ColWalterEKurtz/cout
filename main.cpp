// -----------------------------------------------------------------------------
// main.cpp                                                             main.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief this file holds the main() function
 * @author Col. Walter E. Kurtz
 * @version 2017-04-15
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include <iomanip>
#include <iostream>
#include "message.h"
#include "cli.h"


// -----------------------------------------------------------------------------
// Namespaces                                                         Namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Functions                                                           Functions
// -----------------------------------------------------------------------------

// ----
// help
// ----
/**
 * @brief  This function shows the syntax of the command-line interface.
 */
void help(const string& xname)
{
  // show syntax
  cout << endl;
  cout << "... | " << xname << " [-bhnv] | ..." << endl;
  cout << endl;
  cout << "  -b        create output suitable for binary input files" << endl;
  cout << "  -h        show help and exit" << endl;
  cout << "  -n <str>  use <str> as name of the stream object (default: cout)" << endl;
  cout << "  -v        show version and exit" << endl;
  cout << endl;
}

// -------
// version
// -------
/**
 * @brief  This function shows the program's version.
 */
void version()
{
  cout << "v2018-05-03" << endl;
}

// -----------
// createASCII
// -----------
/**
 * @brief This function creates the output suitable for text files.
 */
int createASCII(const string& name)
{
  char r = 0;  // recent character
  char c = 0;  // current character
  int  n = 0;  // number of characters in the current line

  // read from stdin
  while ( cin.get(c) )
  {
    // CR
    if (c == 13)
    {
      // break line
      if (n == 0)
      {
        // write empty line
        cout << name << " << endl;" << endl;
      }

      else
      {
        // close string
        cout << "\" << endl;" << endl;
      }

      // reset number of characters
      n = 0;
    }

    // LF
    else if (c == 10)
    {
      // recent character differs CR
      if (r != 13)
      {
        // break line
        if (n == 0)
        {
          // write empty line
          cout << name << " << endl;" << endl;
        }

        else
        {
          // close string
          cout << "\" << endl;" << endl;
        }

        // reset number of characters
        n = 0;
      }
    }

    else
    {
      // first character in a new line
      if (n == 0)
      {
        cout << name << " << \"";
      }

      // escape these characters
      if ( (c == 34)    /* double quote */
      ||   (c == 92) )  /* backslash    */
      {
        // print backslash first
        cout << "\\";
      }

      // encode these characters
      if (c == 9)
      {
        // print backslash first
        cout << "\\";

        // print this code character instead
        c = 't';
      }

      // write current character
      cout << c;

      // step counter
      n += 1;
    }

    // update recent character
    r = c;
  }

  // close last line
  if (n > 0)
  {
    // close string
    cout << name << "\" << endl;" << endl;
  }

  // signalize success
  return 0;
}

// ------------
// createBinary
// ------------
/**
 * @brief This function creates the output suitable for binary files.
 */
int createBinary(const string& name)
{
  // number of bytes to read at once
  const std::streamsize length = 1024;

  // buffer of bytes
  char buffer[length];

  // block counter
  unsigned bc = 0;

  // read all bytes from input stream
  while (true)
  {
    // read data from input stream
    cin.read(buffer, length);

    // get number of extracted bytes
    const std::streamsize& gcount = cin.gcount();

    // finished
    if (gcount == 0) break;

    // separate different blocks
    if (bc > 0) cout << endl;

    // print buffer
    cout << "static const char block" << bc << "[" << gcount << "] =" << endl;
    cout << "{" << endl;
    cout << "  ";

    // write byte values
    for(std::streamsize i = 0; i < gcount; i++)
    {
      // print byte (as unsigned integer)
      cout << setw(4) << static_cast<int>( buffer[i] );

      // more bytes to come
      if ((i + 1) < gcount)
      {
        // print separator
        cout << ",";

        // check if 16 numbers have been printed
        if ( ((i + 1) % 16) == 0 )
        {
          // start new line
          cout << endl;

          // print indent
          cout << "  ";
        }

        // continue current line
        else
        {
          // print single space
          cout << " ";
        }
      }
    }

    // break last line
    cout << endl;

    // close buffer definition
    cout << "};" << endl;

    // create write command
    cout << name << ".write(block" << bc << ", " << gcount << ");" << endl;

    // increase block counter
    bc += 1;
  }

  // signalize success
  return 0;
}


// -----------------------------------------------------------------------------
// Main                                                                     Main
// -----------------------------------------------------------------------------

// ----
// main
// ----
/**
 *
 */
int main(int argc, char** argv)
{
  cli cmdl;

  // parse command-line
  if ( cmdl.parse(argc, argv) )
  {
    // check number of positional parameters
    if (cmdl.pparams.size() > 0)
    {
      // notify user
      msg::err("no positional parameters allowed");

      // signalize trouble
      return 1;
    }

    // show help
    if (cmdl.operation == cli::SHOW_HELP)
    {
      help( cmdl.getXName() );
    }

    // show version
    else if (cmdl.operation == cli::SHOW_VERSION)
    {
      version();
    }

    // create real output
    else if (cmdl.operation == cli::DEFAULT)
    {
      // create ASCII output
      if (cmdl.output == cli::ASCII)
      {
        return createASCII(cmdl.name);
      }

      // create BINARY output
      else if (cmdl.output == cli::BINARY)
      {
        return createBinary(cmdl.name);
      }

      else
      {
        msg::err("SNAFU");
      }
    }

    else
    {
      msg::err("SNAFU");
    }
  }

  // signalize trouble
  return 1;
}

