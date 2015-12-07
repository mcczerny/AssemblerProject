//
//		Implementation of the instruction  class.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

/*
NAME

ParseInstruction - Parses the instruction

SYNOPSIS

InstructionType ParseInstruction(string&a_buff)

DESCRIPTION

This function will parse the instruction that is in "a_buff". First checks to see if there is a comment 
in "a_buff" as denoted with a ";". If "a_buff" is only a comment then comment is returned and if part of 
the buffer contains a comment it is removed. Next each word is broken down in "a_buff" and added into the
vector "words". The instruction is broken down depending on the amount of words. The label, OpCode, and 
Operand are detemined depending on the number of words. The OpCode is made all uppercase if it has one.
Then depending on those values the number of OpCode, a bool value determining if the Operand is a number, 
and the number of the operand is filled in. "m_type" is returned and that determines if the instruction 
is an END, Machine language or Assembly instruction. 
*/
Instruction::InstructionType
Instruction::ParseInstruction(string &a_buff)
{
	InitializeValues();
	m_instruction = a_buff;	// Original instruction

	// Checks for ';' at start of buffer
	if (a_buff[0] == ';') { 
		return ST_Comment; 
	}
	// Checks for ';' in buffer
	if (a_buff.find(';')) { 
		a_buff = a_buff.substr(0, a_buff.find(";"));  // Removes comment 
	}
	// Count number of words in buffer and puts each word into vector
	vector<string> words;
	stringstream strStream(a_buff);
	string word;
	int numOfWords = 0;
	while (strStream >> word) {
		numOfWords++;
		words.push_back(word);
	}

	// If 1 word in buffer
	if (numOfWords == 1) {
		transform(words[0].begin(), words[0].end(), words[0].begin(), ::toupper);	// Make OpCode uppercase

		m_OpCode = words[0];
		SetNumOpCode(); //	Sets NumOpCode to approriate number
		CheckOpCode(numOfWords);	// Chceks OpCode for errors

		return m_type;	
	}
	
	// If 2 words in buffer
	else if (numOfWords == 2) {
		transform(words[0].begin(), words[0].end(), words[0].begin(), ::toupper);	// Make OpCode uppercase
		
		m_OpCode = words[0];
		SetNumOpCode(); // Sets NumOpCode to appropriate number
		CheckOpCode(numOfWords);	//	Checks OpCode for errors

		m_Operand = words[1];
		CheckOperand(numOfWords);	//	Checks Operand for errors

		return m_type;
	}
	// If 3 words in buffer
	else if (numOfWords == 3) {
		m_Label = words[0];
		CheckLabel(numOfWords);

		transform(words[1].begin(), words[1].end(), words[1].begin(), ::toupper);	// Make OpCode uppercase

		m_OpCode = words[1];
		SetNumOpCode(); // Sets NumOpCode to appropriate number
		CheckOpCode(numOfWords);

		m_Operand = words[2];
		CheckOperand(numOfWords);

		return m_type;
	}
	else {	// To many operands in instruction
		string emsg = "Error: Extra operands";
		Errors::RecordError(emsg);
	}
};

/*
NAME

LocationNextInstruction - Compute the location of the next instruction

SYNOPSIS

int LocationNextInstruction(int a_loc)

DESCRIPTION

This function will take "a_loc" and use the values taken from parsing the instruction to determine
the location of the next instruction. The location of the next instruction is returned.
*/
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

/*
NAME

SetNumOpCode - To set the number of the OpCode

SYNOPSIS

void SetNumOpCode()

DESCRIPTION

This function will set the number value of the OpCode by using the enumarator OpCodes to determine 
it's number. If the number is not in the enumarator then the number of OpCode it is set to 0. 
*/
void
Instruction::SetNumOpCode() {
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

/*
NAME

InitializeValues - Clears the private members of instruction 

SYNOPSIS

int InitializeValues()

DESCRIPTION

This function will set "m_Label", "m_OpCode", "m_Operand", and "m_instruction" to an empty value. The function 
also sets "m_NumOpCode" and "m_OperandValue" to 0. It sets the bool "m_IsNumbericOperand" equal to false.
*/
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

void
Instruction::CheckLabel(int a_numOfWords)
{
	if (a_numOfWords == 3)
	{
		// Label cannot be more than 10 characters in length
		if (m_Label.size() > 10) {
			string emsg = "Error: Label is to large";
			Errors::RecordError(emsg);
		}
		// Label must start with letter and the remaining may be letters and digits
		if (isalpha(m_Label[0]) == false) {
			string emsg = "Error: Label must start with letter";
			Errors::RecordError(emsg);
		}
	}
}

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
		else {	// Invalid OpCode
			m_OpCode = "??";
			string emsg = "Invalid OpCode;";
			Errors::RecordError(emsg);
			m_type = ST_MachineLanguage;
		}
	}
	if (a_numOfWords == 2)
	{
		if (m_NumOpCode == 0 && m_OpCode == "ORG") {
			m_type = ST_AssemblerInstr;
		}
		if (m_NumOpCode == 0 && m_OpCode != "ORG") {	// Invalid OpCode
			m_OpCode = "??";
			m_type = ST_MachineLanguage;
			string emsg = "Invalid OpCode";
			Errors::RecordError(emsg);
		}
		if(m_NumOpCode != 0) {	// Symbolic OpCode
			m_type = ST_MachineLanguage;
		}
	}


	if (a_numOfWords == 3) {

		if (m_NumOpCode != 0) { // Symbolic OpCode
			m_type = ST_MachineLanguage;
		}
		else{	// Instruction
			if (m_NumOpCode == 0 && m_OpCode == "DS" || m_OpCode == "DC") {
				m_type = ST_AssemblerInstr;
			}
			else {	// Invalid instruction
				m_type = ST_MachineLanguage;
				m_OpCode = "??";
				string emsg = "Invalid OpCode";
				Errors::RecordError(emsg);
			}
		}		
	}
}

void
Instruction::CheckOperand(int a_numOfWords)
{
	if (a_numOfWords == 2)
	{
		if (m_NumOpCode == 0 && m_OpCode == "ORG") {
			if (isalpha(m_Operand[0]) == false)	{
				m_IsNumericOperand = true;
				m_OperandValue = stoi(m_Operand);
			}
			else { //	ORG must have numeric operand
				m_IsNumericOperand = false;
				string emsg = "Error: ORG must have numeric operand";
				Errors::RecordError(emsg);
			}
		}
		
	}
	if (a_numOfWords == 3)
	{
		if (m_OpCode == "DS" || m_OpCode == "DC") {	// These Opcodes must have numeric operand
			if (isalpha(m_Operand[0]) == false) {
				m_IsNumericOperand = true;
				m_OperandValue = stoi(m_Operand);
			}
			else {	// Wrong type of operand
				m_IsNumericOperand = false;
				m_type = ST_AssemblerInstr;
				string emsg = "Error: DS and DC instructions must have a numberic operand";
				Errors::RecordError(emsg);
			}
		}
		else {	// Symbolic OpCodes
				// Symbolic opcodes do not take numberic operands
			if (m_NumOpCode != 0 && isalpha(m_Operand[0]) == false)	//	Operand numeric
			{
				string emsg = "Error: Symbolic OpCode does not take numeric operand";
				Errors::RecordError(emsg);
			}
			//	Operand name is to long
			if (m_Operand.size() > 10) {
				string emsg = "Error: Operand name is to long";
				Errors::RecordError(emsg);
			}
			m_type = ST_MachineLanguage; // can probably remove
		}	
	}
}