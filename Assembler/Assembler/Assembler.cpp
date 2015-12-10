//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/**/
/*
Assembler::Assembler() Assembler::Assembler()

NAME

	Assembler::Assembler - Constructor for the assembler

SYNOPSIS
	
	Assembler::Assembler(int argc, char *argv[]);

		argc	--> the number of strings that make up the command line arguement
		argv	--> the array that contains the strings of argc

DESCRIPTION
	
	This function will pass argc and argv which are command line arguements,to the
	file access constructor. 

RETURNS
	
	Returns no values.

*/
/**/ 
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
	// Nothing else to do here.
}

/**/
/*
Assembler::PassI() Assembler::PassI()

NAME

	Assembler::PassI - Establishes the locations of the symbols

SYNOPSIS

	void PassI();

DESCRIPTION

	This function will establish the location of the symbols and add them to symbol
	table. In order to do this, a line is read from the file and put into "buff".
	The line is then parsed using the object ParseInstruction from the object m_inst.
	It returns the InstructionType and also determines if the instruction contains a
	label. Every line is read from the file and every label found is added to the 
	symbol table. The symbol and the location of it is added to the symbol table. The 
	next location is found using the m_inst object's function LocationNextInstruction.

RETURNS

	Returns no values.

*/
/**/
void 
Assembler::PassI()
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
		if (st == Instruction::ST_End) { return; }

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

/**/
/*
Assembler::DisplaySymbolTable() Assembler::DisplaySymbolTable()

NAME

	Assembler::DisplaySymbolTable - Displays the symbols in the symbol table

SYNOPSIS

	void Assembler::DisplaySymbolTable();

DESCRIPTION

	This function will first output the fact that it will be displaying the symbol 
	table. It will create the column layout to make output look like a table. Then
	it will output the symbol table using the function DisplaySymbolTable using the
	object m_symtab.

RETURNS

	Returns no values.

*/
/**/
void 
Assembler::DisplaySymbolTable()
{
	cout << "Symbol Table:" << endl;
	cout << setw(12) << left << "Symbol#" << setw(12) << left << "Symbol" << setw(12) << left << "Location" << endl;
	m_symtab.DisplaySymbolTable();

	cout << "----------------------------------\n\n" << endl;

}

/**/
/*
Assembler::PassII() Assembler::PassII()

NAME

	Assembler::PassII - Generate a translation

SYNOPSIS

	void Assembler::PassII();

DESCRIPTION

	This function first goes back to the beginning of file using the function rewind
	from the object m_facc. It next uses Errors::InitErrorReporting to clear errors 
	recorded from PassI. The function next goes through the file line by line parsing
	the instruction using ParseInstruction from the object m_inst. Any line with the
	InstructionType of machine or assembly language is translated and outputed using
	the DisplayTranslation function from the object m_trans. 
	
	Once the line has been translated and output, PassII next determines if the
	translated line needs it's "contents" to be inserted into memory of the emulator. 
	PassII then sets loc using  m_inst's LocationNextInstruction.
	
	The function also does error checking such as checking if end was called and where 
	in the file it was called. It also makes sure there is enough memory to keep 
	translating the file.

RETURNS

	Returns no values.

*/
/**/
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
			// End is at end of file
			if (m_inst.GetOpCode() == "END") {
				return;
			}
			// Missing end at end of file
			else {	
				string emsg = "Error: Missing an end statement";
				Errors::RecordError(emsg);
				return;
			}
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);


		if (st == Instruction::ST_End) { 
			// END called at end of file
			if (!m_facc.GetNextLine(buff)) {
				cout << "                        " << m_inst.GetOriginalInstruction() << endl;
				return;
			}
			// END called before end of file 
			else { 
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
		if (m_inst.GetOpCode() == "DC") {
			int contents = stoi(m_trans.getStrContents()); // Used to store int version of contents
			m_emul.InsertMemory(loc, contents); // Inserts into memory
		}

		//	Insert machine language into memory 
		if (st == Instruction::ST_MachineLanguage) {
			int contents = stoi(m_trans.getStrContents());
			m_emul.InsertMemory(loc, contents); // Inserts into memory
		}

		//	Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);

		if (loc > 9999) {
			string emsg = "Error: Insufficient memory for the translation";
			Errors::RecordError(emsg);
			return;
		}
	}
}

/**/
/*
Assembler::RunEmulator() Assembler::RunEmulator()

NAME

	Assembler::RunEmulator - Runs the emulator on the translation

SYNOPSIS

	void Assembler::RunEmulator();

DESCRIPTION
	
	This function will first determine if it can run the program successfully. It will 
	first check if there are any errors recorded using the function Errors::IsEmpty. If
	it finds no errors it procedes to run the function RunProgram from the m_emul object.
	If it does find errors it just diplays the errors using Errors::DisplayErrors and 
	does not emulate the program.

Returns

	Returns no values.

*/
/**/
void 
Assembler::RunEmulator() { 
	
	if (Errors::IsEmpty()) {
		cout << "Results from emulating program: \n" << endl;
		m_emul.RunProgram();
		cout << "\nEnd of emulation" << endl;
	}
	else {
		cout << "\n";
		Errors::DisplayErrors();
	}
}