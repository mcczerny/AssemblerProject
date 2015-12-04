//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#include "stdafx.h"
#include "Errors.h"

// Initializes Vector
vector <string> Errors::m_ErrorMsgs;

/*
NAME

InitErrorReporting - Initializes error report

SYNOPSIS

void InitErrorReporting()

DESCRIPTION

This function will clear the vector that holds the error messages so it will be empty at the 
beginning of PassII
*/
void 
Errors::InitErrorReporting() 
{
	Errors::m_ErrorMsgs.clear();
}

/*
NAME

RecordError - Records an error message

SYNOPSIS

void RecordError(string &a_emsg)

DESCRIPTION

This function will push "a_emsg" to the vector "m_ErrorMsgs".
*/
void 
Errors::RecordError(string &a_emsg)
{
	Errors::m_ErrorMsgs.push_back(a_emsg);
}

/*
NAME

DisplayErrors - Displays the collectedd error message

SYNOPSIS

void DisplayErrors()

DESCRIPTION

This function will push "a_emsg" to the vector "m_ErrorMsgs".
*/
void 
Errors::DisplayErrors() 
{
		for (int i = 0; i < m_ErrorMsgs.size(); i++) {
			cout << m_ErrorMsgs[i] << endl;
		}
}

/*
NAME

IsEmpty - Checks if vector is empty

SYNOPSIS

IsEmpty() 

DESCRIPTION

This function will determine if there are any error messages in the vector "m_ErrorMsgs"
*/
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
