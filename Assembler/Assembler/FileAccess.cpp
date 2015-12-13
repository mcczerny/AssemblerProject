//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*
FileAccess::FileAccess() FileAccess::FileAccess()

NAME

	FileAccess::FileAccess - Opens the file.

SYNOPSIS

	FileAccess::FileAccess(int argc, char *argv[]);

		argc	--> the number of strings that make up the command line arguement.
		argv	--> the array that contains the strings of argc.

DESCRIPTION

	The constructor opens uses command line arguement to open file. There should be 2 arguements in
	order to open the command line arguement. If for whatever reason the file can not open an error
	message is displayed and the program exits.

RETURNS

	Returns no values.

*/
/**/
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
		cerr << "Source file could not be opened, assembler terminated." << endl;
		exit(1);
	}
}

/**/
/*
FileAccess::~FileAccess() FileAccess::~FileAccess()

NAME

	FileAccess::~FileAccess - Closes the file.

SYNOPSIS

	FileAccess::~FileAccess();

DESCRIPTION

	The destructor closes the file m_sfile.

Returns

	Returns no values.

*/
/**/
FileAccess::~FileAccess()
{
	m_sfile.close();
}

/**/
/*
FileAccess::GetNextLine() FileAccess::GetNextLine()

NAME

	FileAccess::GetNextLine - Get the next line from the source file.

SYNOPSIS

	bool FileAccess::GetNextLine(string &a_buff);

		a_buff --> The buffer that the line is put into.

DESCRIPTION

	This function will get the next line in the file and put it in a_buff.

RETURNS

	Returns false if m_sfile is at end of file. Returns true indicating it could get the next line.

*/
/**/
bool 
FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false;


	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}

/**/
/*
FileAccess::Rewind() FileAccess::Rewind()

NAME

	FileAccess::Rewind - Put the file pointer back to the beginning of the file.

SYNOPSIS

	void FileAccess::Rewind();

DESCRIPTION

	This function will clear the end of file flag and go back to the beginning of the file.

RETURNS

	Returns no values.

*/
/**/
void 
FileAccess::rewind()
{
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}
