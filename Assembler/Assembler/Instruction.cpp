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
		if (words[0] == "HALT") {
			m_OpCode = words[0];
			m_NumOpCode = HALT;
			m_type = ST_MachineLanguage;
			return m_type;
		}
		if (words[0] == "END") {
			m_OpCode = words[0];
			m_type = ST_AssemblerInstr;
			return m_type;
		}
		else {
			string emsg = "Error: Invalid OpCode";
			Errors::RecordError(emsg);
		}
	}
	// If 2 words in buffer
	else if (numOfWords == 2) {
		transform(words[0].begin(), words[0].end(), words[0].begin(), ::toupper);	// Make OpCode uppercase
		m_OpCode = words[0];
		SetNumOpCode(); // Sets NumOpCode to appropriate number

		m_Operand = words[1];
		if (isdigit(m_Operand[0])) {
			m_IsNumericOperand = true;
			m_OperandValue = stoi(m_Operand);
			m_type = ST_AssemblerInstr;
			return m_type;
		}
		else { 
			m_type = ST_MachineLanguage;
			return m_type;
		}
	}
	// If 3 words in buffer
	else if (numOfWords == 3) {
		m_Label = words[0];
		transform(words[1].begin(), words[1].end(), words[1].begin(), ::toupper);	// Make OpCode uppercase
		m_OpCode = words[1];
		SetNumOpCode(); // Sets NumOpCode to appropriate number

		m_Operand = words[2];
		if (isdigit(m_Operand[0])) {
			m_IsNumericOperand = true;
			m_OperandValue = stoi(m_Operand);
			m_type = ST_AssemblerInstr;
			return m_type;
		}
		else {
			m_type = ST_MachineLanguage;
			return m_type;
		}
	}
	else {
		string emsg = "Error: Extra operands";
		Errors::RecordError(emsg);
	}
	// Checks for valid OpCode
	if (m_NumOpCode == 0 && m_IsNumericOperand == false) {
		string emsg = "Error: Invalid OpCode";
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
		m_NumOpCode = 0;
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