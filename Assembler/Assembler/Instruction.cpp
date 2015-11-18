//
//		Implementation of the instruction  class.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"



// Parse the Instruction.
Instruction::InstructionType
Instruction::ParseInstruction(string &a_buff)
{
	// Set/Clear private variables
	m_Label = "";           
	m_OpCode = "";       
	m_Operand = "";      
	m_instruction = a_buff;   
	m_NumOpCode = 0;     
	m_IsNumericOperand = false;
	m_OperandValue = 0;  

	// Checks for comment at start of buffer
	if (a_buff[0] == ';') { return ST_Comment; }

	// Checks for comment in buffer
	if (a_buff.find(';')) { a_buff = a_buff.substr(0, a_buff.find(";")); }

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
			m_OpCode = "HALT";
			m_NumOpCode = HALT;
			m_type = ST_MachineLanguage;
			return m_type;
		}
		if (words[0] == "END") {
			m_OpCode = "END";
			m_type = ST_AssemblerInstr;
			return m_type;
		}
	}

	// If 2 words in buffer
	if (numOfWords == 2) {
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
	if (numOfWords == 3) {
		m_Label = words[0];
		transform(words[1].begin(), words[1].end(), words[1].begin(), ::toupper);	// Make OpCode uppercase
		m_OpCode = words[1];

		// Sets NumOpCode to appropriate number
		SetNumOpCode();

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
};

// Compute the location of the next instruction.
int
Instruction::LocationNextInstruction(int a_loc)
{
	if (m_IsNumericOperand == true) {

		if (m_OpCode == "ORG") { return m_OperandValue; }
		if (m_OpCode == "DC") { return a_loc + 1; }
		if (m_OpCode == "DS") { return a_loc + m_OperandValue; }
	}

	if (m_IsNumericOperand == false) { return a_loc + 1; }
};

void
Instruction::SetNumOpCode() {

	if (m_OpCode == "ADD") { m_NumOpCode = ADD; }
	if (m_OpCode == "SUB") { m_NumOpCode = SUB; }
	if (m_OpCode == "MULT") { m_NumOpCode = MULT; }
	if (m_OpCode == "DIV") { m_NumOpCode = DIV; }
	if (m_OpCode == "LOAD") { m_NumOpCode = LOAD; }
	if (m_OpCode == "STORE") { m_NumOpCode = STORE; }
	if (m_OpCode == "READ") { m_NumOpCode = READ; }
	if (m_OpCode == "WRITE") { m_NumOpCode = WRITE; }
	if (m_OpCode == "B") { m_NumOpCode = B; }
	if (m_OpCode == "BM") { m_NumOpCode = BM; }
	if (m_OpCode == "BZ") { m_NumOpCode = BZ; }
	if (m_OpCode == "BP") { m_NumOpCode = BP; }
	
}
