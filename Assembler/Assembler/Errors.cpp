//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#include "stdafx.h"
#include "Errors.h"

// Initializes Vector
vector <string> Errors::m_ErrorMsgs;

// Initializes Error Report
void Errors::InitErrorReporting() {

	Errors::m_ErrorMsgs.clear();

}

// Records an error message.
void Errors::RecordError(string &a_emsg)
{
	Errors::m_ErrorMsgs.push_back(a_emsg);
}

// Displays the collected error message.
void Errors::DisplayErrors() {

	if (!m_ErrorMsgs.empty()) {
		for (int i = 0; i < m_ErrorMsgs.size(); i++)
		{
			cout << m_ErrorMsgs[i] << endl;
		}
	}
	else {
		return;
	}
}

