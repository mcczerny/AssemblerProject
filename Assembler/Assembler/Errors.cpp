//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#include "stdafx.h"
#include "Errors.h"

// Initializes Vector
vector <string> Errors::m_ErrorMsgs;

/**/
/*
Errors::InitErrorReporting() Errors::InitErrorReporting()

NAME

	Errors::InitErrorReporting - Initializes error reports.

SYNOPSIS

	void Errors::InitErrorReporting();

DESCRIPTION

	This function will clear the vector that holds the error messages.

RETURNS

	Returns no values.

*/
/**/
void 
Errors::InitErrorReporting() 
{
	Errors::m_ErrorMsgs.clear();
}

/**/
/*
Errors::RecordError() Errors::RecordError()

NAME

	Errors::RecordError - Records an error message.

SYNOPSIS

	void Errors::RecordError(string &a_emsg);

		a_emsg ->> The message to be recorded into m_ErrorMsgs.

DESCRIPTION

	This function will push a_emsg to the vector m_ErrorMsgs.

RETURNS

	Returns no values.
*/
/**/
void 
Errors::RecordError(string &a_emsg)
{
	Errors::m_ErrorMsgs.push_back(a_emsg);
}

/**/
/*
Errors::DisplayErrors() Errors::DisplayErrors()

NAME

	DisplayErrors - Displays the collected error messages.

SYNOPSIS

	void Errors::DisplayErrors();

DESCRIPTION

	This function will output the error messages stored in m_ErrorMsgs.

RETURNS

	Returns no values.

*/
/**/
void 
Errors::DisplayErrors() 
{
		for (int i = 0; i < m_ErrorMsgs.size(); i++) {
			cout << m_ErrorMsgs[i] << endl;
		}
}

/**/
/*
Errors::IsEmpty() Errors::IsEmpty()

NAME

	Errors::IsEmpty - Checks if vector is empty.

SYNOPSIS

	bool Errors::IsEmpty();

DESCRIPTION

	This function will determine if the vector is empty.

RETURNS

	Returns true if the vector is empty and false if the vector is not empty.

*/
/**/
bool 
Errors::IsEmpty()
{
	if (m_ErrorMsgs.empty()){
		return true;
	}
	else {
		return false;
	}
}
