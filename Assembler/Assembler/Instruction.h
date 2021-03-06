//
// Class to parse and provide information about instructions
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

	Instruction() { };
	~Instruction() { };

	// Enumarator for type of instruction
	enum InstructionType {
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,          // Comment or blank line
		ST_End                    // end instruction.
	};
	
	 //Enumarator for OpCodes
	enum OpCodes {
		ADD = 1,
		SUB = 2,
		MULT = 3,
		DIV = 4,
		LOAD = 5,
		STORE = 6,
		READ = 7,
		WRITE = 8,
		B = 9,
		BM = 10,
		BZ = 11,
		BP = 12,
		HALT = 13
	};

// Parse the Instruction.
InstructionType 
ParseInstruction(string &a_buff);

// Compute the location of the next instruction.
int 
LocationNextInstruction(int a_loc);

// To set number of OpCode.
void
SetNumOpCode();

// Clears the private members of instruction.
void 
InitializeValues();

// Does error checking for the label.
void
CheckLabel(int a_numOfWords);

// Does error checking for the OpCode.
void
CheckOpCode(int a_numOfWords);

// Does error checking for the operand.
void
CheckOperand(int a_numOfWords);

// To access the label
inline string &
GetLabel() { return m_Label; };

// To determine if a label is blank.
inline bool
isLabel() { return !m_Label.empty(); }

// To access the OpCode
inline string &
GetOpCode() { return m_OpCode; }

// To access number of OpCode
int &
GetNumOpCode() { return m_NumOpCode; }

// To access operand
inline string
GetOperand() { return m_Operand; }

// To access operand value
int
GetOperandValue() { return m_OperandValue; }

// To access original instruction
string
GetOriginalInstruction() { return m_instruction; }

InstructionType &
GetInstructionType() { return m_type; }

private:


	// The elemements of a instruction
	string m_Label;            // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.

	string m_instruction;    // The original instruction.

							 // Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand;// == true if the operand is numeric.
	int m_OperandValue;   // The value of the operand if it is numeric.
};

