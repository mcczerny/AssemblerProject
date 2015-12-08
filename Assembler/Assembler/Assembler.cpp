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


/*
NAME

PassI - Establishes the location of the labels

SYNOPSIS

void PassI()

DESCRIPTION

This function will add labels to symbol table. First command line arguement opens the file and the function "GetNextLine" 
from the FileAccess class returns the line to "buff" the instruction. "buff" is parsed line by line using the function 
"ParseInstruction" from the Instruction class. It returns the InstructionType and also determines if the instruction 
contains a label. A for loop continues to GetNextLine of the file until it has reached the end of file. Every instruction
that has a label is added to a symbol table using AddSymbol. Before the end of the for loop, the LocationNextInstruction
function from the Instruction class that determines where in the symbol table the labels are placed.
*/
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

						// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) { return; }

		if (buff == "") { continue; }

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

/*
NAME

DisplaySymbolTable - Displays the Symbol Table

SYNOPSIS

void DisplaySymbolTable()

DESCRIPTION

This function will output the symbol table using the function DisplaySymbolTable from the class SymTab
*/
void 
Assembler::DisplaySymbolTable()
{
	cout << "Symbol Table:" << endl;
	cout << setw(12) << left << "Symbol#" << setw(12) << left << "Symbol" << setw(12) << left << "Location" << endl;
	m_symtab.DisplaySymbolTable();

	cout << "----------------------------------\n\n" << endl;

}

/*
NAME

PassII - Translates assembly language to machine language

SYNOPSIS

void PassII()

DESCRIPTION

This function first need to go back to the beginning of the file used in Pass I. We use the function rewind from the FileAccess class
to do this. The function first clears all errors found from PassI using the function InitErrorReporting from the Errors class. It then 
runs through the for loop just as PassI does, but nothing is added to the symbol table. The function translates and outputs the instruction
first by parsing the instruction. Any line with the InstructionType of machine or assembly language is translated and outputed using the
DisplayTranslation function from the translation class. Once the line has been translated and output, PassII next determines determines
if the translated line needs it's "contents" to be inserted into memory of the emulator.
*/
void 
Assembler::PassII()
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
				string emsg = "Error: End is missing";
				Errors::RecordError(emsg);
				return;
			}
		}

		if (buff == "") { continue; }

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		if (st == Instruction::ST_End) {
			if (m_facc.GetNextLine(buff) == false) {
				return;
			} // END called at end of file

			else { // END called before end of file
				string emsg = "Error: End is not at end of file";
				Errors::RecordError(emsg);
				return;
			}
		}

		//	Outputs translation if comment
		if (st == Instruction::ST_Comment) { cout << "                        " << m_inst.GetOriginalInstruction() << endl; }

		// Labels can only be on machine language and assembler language instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// Outputs translation of machine and assembly language
		m_trans.DisplayTranslation(st, m_inst, m_symtab, loc);

		//	Insert DC assembly instruction into memory
		//	DC is the only assembly instruction put into memory
		if (m_inst.GetOpCode() == "DC")
		{
			int contents = stoi(m_trans.getStrContents()); // Used to store int version of contents
			m_emul.insertMemory(loc, contents); // Inserts into memory
		}

		//	Insert machine language into memory 
		if (st == Instruction::ST_MachineLanguage) {
			int contents = stoi(m_trans.getStrContents());
			m_emul.insertMemory(loc, contents); // Inserts into memory
		}

		//	Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);

		if (loc > 9999)
		{
			string emsg = "Error: Ran out of memory";
			Errors::RecordError(emsg);
			return;
		}
	}
}

void 
Assembler::RunEmulator() { 
	
	if (Errors::IsEmpty()) {
		cout << "Results from emulating program: \n" << endl;
		m_emul.runProgram();
		cout << "\nEnd of emulation" << endl;
	}
	else {
		cout << "\n";
		Errors::DisplayErrors();
	}
}