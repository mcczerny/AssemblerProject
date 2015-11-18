//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#include "stdafx.h"
#include "Errors.h"

	// Initializes error reports.
void InitErrorReporting();


// Records an error message.
void Errors::RecordError(string &a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
}

// Displays the collected error message.
//void Errors::DisplayErrors() {
//	
//	for (int i = 0; i < m_ErrorMsgs.size(); i++)
//	{
//		cout << m_ErrorMsgs[i];
//	}
//}

