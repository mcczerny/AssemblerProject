//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// 
//
#ifndef _ERRORS_H
#define _ERRORS_H

class Errors {

public:
	
	// Initializes error reports.
	static void InitErrorReporting();

	// Records an error message.
	static void RecordError(string &a_emsg);

	// Displays the collected error messages.
	static void DisplayErrors();

	// Checks if vector is empty.
	static bool IsEmpty();

private:

	static vector<string> m_ErrorMsgs;
};
#endif