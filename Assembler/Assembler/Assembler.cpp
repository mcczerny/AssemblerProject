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
		if (!m_facc.GetNextLine(buff)) { return; }
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
	m_facc.rewind();	// Goes baack to beginning of file
	Errors::InitErrorReporting(); // Clears any errors from PassI

	cout << "\nTranslation of Program:\n" << endl;
	cout << setw(12) << left << "Location" << setw(12) << left << "Contents" << setw(12) << left << "Original Statement" << endl;

	// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			if (m_inst.GetOpCode() == "END") { return; }

			else {	// Missing END at end of file
				string emsg = "Error: End has not been called at end of file";
				Errors::RecordError(emsg);
				return;
			}
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		if (st == Instruction::ST_End) {
			if (!m_facc.GetNextLine(buff)) { return; } // END called at end of file

			else { // END called before end of file
				string emsg = "Error: End is not at end of file";
				Errors::RecordError(emsg);
				return;
			}
		}
		if (st == Instruction::ST_Comment) { cout << "                        " << m_inst.GetOriginalInstruction() << endl; }

		// Labels can only be on machine language and assembler language instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		m_trans.DisplayTranslation(st, m_inst, m_symtab, m_emul, loc);

		int contents = stoi(m_trans.getStrContents());
		m_emul.insertMemory(loc, contents); // Inserts into memory

		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
		}
		
	}

void Assembler::RunEmulator() { 
	
	if (Errors::IsEmpty()) {
		cout << "Results from emulating program: \n" << endl;
		m_emul.runProgram();
		cout << "\nEnd of emulation" << endl;
	}
	else {
		Errors::DisplayErrors();
	}
}