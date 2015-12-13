//
//		Implementation of the instruction  class.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

/**/
/*
Instruction::ParseInstruction() Instruction::ParseInstruction()

NAME

	Instruction::ParseInstruction - Parses the instruction.

SYNOPSIS

	Instruction::InstructionType Instruction::ParseInstruction(string&a_buff);

		a_buff --> the buffer for the line currently being worked on.

DESCRIPTION

	This function will parse the instruction that is in a_buff. First checks to see if there is a 
	comment in a_buff as denoted with a ";". If a_buff is only a comment then comment is returned
	and if part of the buffer contains a comment it is removed. Next each word is broken down in 
	a_buff and added into the vector words. The instruction is broken down depending on the amount
	of words and he label, OpCode, and Operand are detemined. Every label, OpCode and Operand are 
	then checked with CheckLabel, CheckOpCode, and the CheckOperand functions repectively that
	determine if they are valid, doing the error checking and determining the return type among 
	other things.

RETURNS

	m_type is returned. It determines if the line that was passed in a_buff is a comment, an end,
	machine language or assembly instruction.

*/
/**/
Instruction::InstructionType
Instruction::ParseInstruction(string &a_buff)
{
	InitializeValues();
	m_instruction = a_buff;	// Original instruction.
	
	// Checks for ';' at start of buffer.
	if (a_buff[0] == ';') { 
		m_type = ST_Comment;
		return m_type; 
	}

	// Checks for ';' in buffer.
	if (a_buff.find(';')) { 
		a_buff = a_buff.substr(0, a_buff.find(";"));  // Removes comment.
	}

	// Checks for empty line.
	if (a_buff == "") {
		m_type = ST_Comment;
		return m_type;
	}

	// Checks for line with only spaces.
	if (a_buff.find_first_not_of(' ') == string::npos) {
		m_type = ST_Comment;
		return m_type;
	}

	// Count number of words in buffer and puts each word into vector.
	vector<string> words;
	stringstream strStream(a_buff);
	string word;
	int numOfWords = 0;
	while (strStream >> word) {
		numOfWords++;
		words.push_back(word);
	}

	// If 1 word in buffer.
	if (numOfWords == 1) {
		// Make OpCode uppercase.
		transform(words[0].begin(), words[0].end(), words[0].begin(), ::toupper);	

		m_OpCode = words[0];
		SetNumOpCode(); //	Sets NumOpCode to approriate number.
		CheckOpCode(numOfWords);	// Checks OpCode for errors.

		return m_type;	
	}
	
	// If 2 words in buffer
	else if (numOfWords == 2) {
		// Make OpCode uppercase.
		transform(words[0].begin(), words[0].end(), words[0].begin(), ::toupper);
		
		m_OpCode = words[0];
		SetNumOpCode(); // Sets NumOpCode to appropriate number.
		CheckOpCode(numOfWords);	//	Checks OpCode for errors.

		m_Operand = words[1];
		CheckOperand(numOfWords);	//	Checks Operand for errors.

		return m_type;
	}
	
	// If 3 words in buffer OR to many words.
	else {
		if (numOfWords > 3){
			string emsg = "Error: Extra operands in instruction";
			Errors::RecordError(emsg);
		}

		m_Label = words[0];
		CheckLabel(numOfWords);

		// Make OpCode uppercase.
		transform(words[1].begin(), words[1].end(), words[1].begin(), ::toupper);	

		m_OpCode = words[1];
		SetNumOpCode(); // Sets NumOpCode to appropriate number.
		CheckOpCode(numOfWords);

		m_Operand = words[2];
		CheckOperand(numOfWords);

		return m_type;
	}
};

/**/
/*
Instruction::LocationNextInstruction() Instruction::LocationNextInstruction()

NAME

	Instruction::LocationNextInstruction - Compute the location of the next instruction.

SYNOPSIS

	int Instruction::LocationNextInstruction(int a_loc);

		a_loc ->> The current location.

DESCRIPTION

	This function will take a_loc and use the values taken from parsing the instruction to determine
	the location of the next instruction. The location of the next instruction is returned.

RETURNS

	Returns the location of the next instruction.

*/
/**/
int
Instruction::LocationNextInstruction(int a_loc)
{
	if (m_IsNumericOperand == true) {

		if (m_OpCode == "ORG") { return m_OperandValue; }
		if (m_OpCode == "DC") { return a_loc + 1; }
		if (m_OpCode == "DS") { return a_loc + m_OperandValue; }
	}
	else { return a_loc + 1; }
};

/**/
/*
Instruction::SetNumOpCode() Instruction::SetNumOpCode()

NAME

	Instruction::SetNumOpCode - To set the number of the OpCode.

SYNOPSIS

	void Instruction::SetNumOpCode();

DESCRIPTION

	This function will set the number value of the OpCode by using the enumarator OpCodes to determine
	it's number. If the number is not in the enumarator then the number of OpCode is set to 0.

RETURNS

	Returns no values.

*/
/**/
void
Instruction::SetNumOpCode() 
{
	if (m_OpCode == "ADD") { m_NumOpCode = ADD; }
	else if (m_OpCode == "SUB") { m_NumOpCode = SUB; }
	else if (m_OpCode == "MULT") { m_NumOpCode = MULT; }
	else if (m_OpCode == "DIV") { m_NumOpCode = DIV; }
	else if (m_OpCode == "LOAD") { m_NumOpCode = LOAD; }
	else if (m_OpCode == "STORE") { m_NumOpCode = STORE; }
	else if (m_OpCode == "READ") { m_NumOpCode = READ; }
	else if (m_OpCode == "WRITE") { m_NumOpCode = WRITE; }
	else if (m_OpCode == "B") { m_NumOpCode = B; }
	else if (m_OpCode == "BM") { m_NumOpCode = BM; }
	else if (m_OpCode == "BZ") { m_NumOpCode = BZ; }
	else if (m_OpCode == "BP") { m_NumOpCode = BP; }
	else if (m_OpCode == "HALT") { m_NumOpCode = HALT; }
	else {
		m_NumOpCode = 0;	// Must be assembly instruction or invalid OpCode
	}
}

/**/
/*
Instruction::InitializeValues() Instruction::InitializeValues()

NAME

	Instruction::InitializeValues - Clears the private members of instruction.

SYNOPSIS

	int Instruction::InitializeValues();

DESCRIPTION

	This function will set m_Label, m_OpCode, m_Operand, and m_instruction to an empty value. The
	function also sets m_NumOpCode and m_OperandValue to 0. It sets the bool m_IsNumbericOperand
	equal to false.

RETURNS

	Returns no values.

*/
/**/
void 
Instruction::InitializeValues() {
	m_Label = "";
	m_OpCode = "";
	m_Operand = "";
	m_instruction = "";
	m_NumOpCode = 0;
	m_IsNumericOperand = false;
	m_OperandValue = 0;
}

/**/
/*
Instruction::CheckLabel() Instruction::CheckLabel()

NAME

Instruction::CheckLabel - Does error checking for the label.

SYNOPSIS

	void Instruction::CheckLabel(int a_numOfWords);

		a_numOfWords --> The number of words detected from the line.

DESCRIPTION

	The function will determine if the label is to large or if the label starts with a ltter. If 
	the label is bigger than 10 characters then it inserts an error. If the label starts with a
	number then it inserts an error.

RETURNS

	Returns no values.

*/
/**/
void
Instruction::CheckLabel(int a_numOfWords)
{
	if (a_numOfWords == 3)
	{
		// Label cannot be more than 10 characters in length.
		if (m_Label.size() > 10) {
			string emsg = "Error: Label is to large";
			Errors::RecordError(emsg);
		}
		// Label must start with letter and the remaining may be letters and digits.
		if (isalpha(m_Label[0]) == false) {
			string emsg = "Error: Label must start with letter";
			Errors::RecordError(emsg);
		}
	}
}

/**/
/*
Instruction::CheckOpCode() Insstruction::CheckOpCode()

NAME

	Instruction::CheckOpCode - Does error checking for the OpCode.

SYNOPSIS

	void Instruction::CheckOpCode(int a_numOfWords);

		a_numOfWords --> The number of words detected from the line.

DESCRIPTION

	This function will determine the instruction type depending on the number of words and the OpCode.
	It will also check if it is a valid OpCode and if it is not then it will replace it with "??".

RETURNS

	Returns no values.

*/
/**/
void
Instruction::CheckOpCode(int a_numOfWords)
{
	if (a_numOfWords == 1) {
		if (m_OpCode == "HALT" || m_OpCode == "END") {
			if (m_OpCode == "HALT")
			{
				m_type = ST_MachineLanguage;
			}
			if (m_OpCode == "END")
			{
				m_type = ST_End;
			}
		}
		// Invalid OpCode.
		else {	
			m_OpCode = "??";
			string emsg = "Invalid OpCode;";
			Errors::RecordError(emsg);
			m_type = ST_MachineLanguage;
		}
	}

	if (a_numOfWords == 2) {
		if (m_NumOpCode == 0 && m_OpCode == "ORG") {
			m_type = ST_AssemblerInstr;
		}
		// Invalid OpCode.
		if (m_NumOpCode == 0 && m_OpCode != "ORG") {	
			m_OpCode = "??";
			m_type = ST_MachineLanguage;
			string emsg = "Invalid OpCode";
			Errors::RecordError(emsg);
		}
		// Symbolic OpCode.
		if(m_NumOpCode != 0) {	
			m_type = ST_MachineLanguage;
		}
	}


	if (a_numOfWords == 3) {
		// Symbolic OpCode.
		if (m_NumOpCode != 0) { 
			m_type = ST_MachineLanguage;
		}
		// Instruction.
		else{	
			if (m_NumOpCode == 0 && m_OpCode == "DS" || m_OpCode == "DC") {
				m_type = ST_AssemblerInstr;
			}
			// Invalid instruction.
			else {	
				m_type = ST_MachineLanguage;
				m_OpCode = "??";
				string emsg = "Invalid OpCode";
				Errors::RecordError(emsg);
			}
		}		
	}
}

/**/
/*
Instruction::CheckOperand() Instruction::CheckOperand()

NAME

	Instruction::CheckOperand - Does error checking for the operand.

SYNOPSIS

	void Instruction::CheckOperand(int a_numOfWords);

		a_numOfWords --> The number of words detected from the line.

DESCRIPTION

	This function will determine if the operand is a number or not. It will also run error checking,
	making sure that the operands are the correct type for the given OpCode.

RETURNS

	Returns no values.

*/
/**/
void
Instruction::CheckOperand(int a_numOfWords)
{
	if (a_numOfWords == 2) { 
		if (m_NumOpCode == 0 && m_OpCode == "ORG") {
			if (isalpha(m_Operand[0]) == false) {
				m_IsNumericOperand = true;
				m_OperandValue = stoi(m_Operand);
			}
			// ORG must have numeric operand.
			else { //	
				m_IsNumericOperand = false;
				string emsg = "Error: ORG must have numeric operand";
				Errors::RecordError(emsg);
			}
			return;
		}
	}
	
	if (a_numOfWords == 3){
		// These Opcodes must have numeric operand.
		if (m_OpCode == "DS" || m_OpCode == "DC") {	
			if (isalpha(m_Operand[0]) == false) {
				m_IsNumericOperand = true;
				m_OperandValue = stoi(m_Operand);
			}
			// Wrong type of operand
			else {	
				m_IsNumericOperand = false;
				m_type = ST_AssemblerInstr;
				string emsg = "Error: DS and DC instructions must have a numberic operand";
				Errors::RecordError(emsg);
			}
			return;
		}
	}

	// Symbolic Opcodes should not have numeric operands.
	//	Operand is numeric.
	if (m_NumOpCode != 0 && isalpha(m_Operand[0]) == false) {	
		string emsg = "Error: Symbolic OpCode does not take numeric operand";
		Errors::RecordError(emsg);
	}
	//	Operand name is to long.
	if (m_Operand.size() > 10) {
		string emsg = "Error: Operand name is to long";
		Errors::RecordError(emsg);
	}
	// Label must start with letter and the remaining may be letters and digits.
	if (isalpha(m_Operand[0]) == false) {
		string emsg = "Error: Label must start with letter";
		Errors::RecordError(emsg);
	}
}