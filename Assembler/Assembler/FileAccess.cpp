//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

FileAccess - Opens the file

SYNOPSIS

FileAccess()

DESCRIPTION

The constructor opens uses command line arguement to open file. There should be 2 arguements in order to 
open the command line arguement. If for whatever reason the file can not open an error message is displayed.
*/
FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}

/*
NAME

~FileAccess - Closes the file

SYNOPSIS

~FileAccess()

DESCRIPTION

The destructor closes the file "m_sfile"
*/
FileAccess::~FileAccess()
{
	m_sfile.close();
}

/*
NAME

GetNextLine - Get the next line from the source file.

SYNOPSIS

void GetNextLine(string &a_buff)

DESCRIPTION

This function will get the next line in the file and put it in "a_buff". If end of file is hit, 
false is returned and if it successfully gets the next line, true is returned.
*/
bool 
FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false;

	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}

/*
NAME

rewind - Put the file pointer back to the beginning of the file.

SYNOPSIS

void rewind()

DESCRIPTION

This function will clear the end of file flag and go back to the beginning of the file 
to be read again in PassII
*/
void 
FileAccess::rewind()
{
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}
