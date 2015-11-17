//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
	// Nothing else to do here.
}
// Destructor currently does nothing.  You might need to add something.
Assembler::~Assembler()
{
}
// Pass I establishes the location of the labels.  You will do a better function comments.
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

						// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}

// Displays Symbol Table
void Assembler::DisplaySymbolTable()
{
	cout << "Symbol Table:" << endl;
	cout << setw(12) << left << "Symbol#" << setw(12) << left << "Symbol" << setw(12) << left << "Location" << endl;
	m_symtab.DisplaySymbolTable();

	cout << "----------------------------------\n\n" << endl;

}

// Pass II translates assembly language to machine language
void Assembler::PassII()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.
	m_facc.rewind();
	cout << "\nTranslation of Program:\n" << endl;
	cout << setw(12) << left << "Location" << setw(12) << left << "Contents" << setw(12) << left << "Original Statement" << endl;

						// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			string errorMissingEnd = "Error: Missing an end statement";
			//Errors::RecordError(errorMissingEnd);

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		
		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.

		if (st == Instruction::ST_End) { return; }

		if (st == Instruction::ST_Comment) { cout << "                        " << m_inst.GetOriginalInstruction() << endl; }

		// Labels can only be on machine language and assembler language instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		string strContents;	//Used to store "string" version of contents

		// ASSEMBLER INSTRUCTIONS INCLUDE DC, DS, ORG, END. Operand is a number.
		if (st == Instruction::ST_AssemblerInstr) {

			// If instruction has a label: DC, DS 
			if (m_inst.isLabel()) {

				if (m_inst.GetOpCode() == "DS") { cout << setw(24) << left << loc << m_inst.GetOriginalInstruction() << endl; }

				if (m_inst.GetOpCode() == "DC") {
					ostringstream intToString;
					intToString << m_inst.GetOperandValue();
					strContents = intToString.str();
					for (int i = strContents.size(); i < 6; i++) { strContents.insert(0, "0"); }
					cout << setw(12) << left << loc << setw(12) << left << strContents << m_inst.GetOriginalInstruction() << endl;

					int contents = stoi(strContents); // Used to store int version of contents
					m_emul.insertMemory(loc, contents); // Inserts into memory
				}
			}

			// If instruction does not have a label: ORG, END.
			if (m_inst.isLabel() == false) {
				if (m_inst.GetOpCode() == "ORG") { cout << setw(24) << left << loc << m_inst.GetOriginalInstruction() << endl; }

				if (m_inst.GetOpCode() == "END") { cout << "                        " << m_inst.GetOriginalInstruction() << endl; }
			}
		}

		// MACHINE INSTRUCTIONS ARE ANY OP CODE 
		if (st == Instruction::ST_MachineLanguage) {

			int pointerLoc; // Used to pointer location from symbol table
			ostringstream intToString;	// Used to resize contents to 6 bits
			intToString << m_inst.GetNumOpCode();
			strContents = intToString.str();

			if (m_inst.GetOpCode() == "HALT") {
				for (int i = strContents.size(); i < 6; i++) { strContents.insert(2, "0"); }	// Inserts 0's contents to make 6 bits
				cout << setw(12) << left << loc << setw(12) << left << strContents << m_inst.GetOriginalInstruction() << endl;
			}
			// For All Other OpCodes
			else {
				m_symtab.LookupSymbol(m_inst.GetOperand(), pointerLoc);		

				if (strContents.size() == 1) { strContents.insert(0, "0"); }	// If OpCode is only 1 char a 0 is added to front

				intToString.str("");	// Clear stringstream for use again
				intToString << pointerLoc;
				strContents = strContents + intToString.str();
				
				if (strContents.size() == 5) { strContents.insert(2, "0"); }	// If the strContents is size 5, we need to add a 0 after the OpCode.
				cout << setw(12) << left << loc << setw(12) << left << strContents << setw(12) << left << m_inst.GetOriginalInstruction() << endl;
			}
			int contents = stoi(strContents);
			m_emul.insertMemory(loc, contents); // Add location and contents into VC3600 MEMORY
		}

		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}

void Assembler::RunEmulator() { 
	
	cout << "Results from emulating program: \n" << endl;

	m_emul.runProgram();

}