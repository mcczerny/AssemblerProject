//
//		Implementation of the translation class.
//
#include "stdafx.h"
#include "Translation.h"
#include "Errors.h"

/**/
/*
Translation::DisplayTranslation() Translation::DisplayTranslation()

NAME

	Translation::DisplayTranslation - Translates machine and assembly language code and outputs it
	to console.

SYNOPSIS

	void Translation::DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst,
										 SymbolTable a_symtab, int a_loc);

		a_st		--> The instruction type of the line.
		a_inst		--> The Instruction object.
		a_symtab	--> The SymTab object.
		a_loc		--> The location of instruction.

DESCRIPTION

	This function will output the translation, and the original instruction. We use a_st to determine
	if instruction is either assembly or machine language code. Then a_inst will be used to retrieve
	the parsed data. Then, a_symtab will be used to retrieve the "contents" that will be inserted
	into emulator are generated if needed.  Finally, a_loc, the generated contents (if needed), and
	the original instruction are formated is printed out to user.

RETURNS

	Returns no values.
*/
/**/
void Translation::DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, int a_loc) {

	ostringstream intToString;	// Used to resize contents to 6 bits.

	if (a_st == Instruction::ST_AssemblerInstr) {
		// DS instruction.
		if (a_inst.GetOpCode() == "DS") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }

		// DC instruction .
		if (a_inst.GetOpCode() == "DC") {
			// Constant is to big for memory.
			if (a_inst.GetOperandValue() > 99999)
			{
				string emsg = "Error: Constant too large for VC 3600 memory";
				Errors::RecordError(emsg);
				ostringstream intToString;
				intToString << a_inst.GetOperandValue();
				m_strContents = intToString.str();
				m_strContents = m_strContents.substr(0, 6);  // Keeps only first 6 digits of defined constant.
				cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
			}
			// Correct Case.
			else {
				ostringstream intToString;
				intToString << a_inst.GetOperandValue();
				m_strContents = intToString.str();
				for (int i = m_strContents.size(); i < 6; i++) { m_strContents.insert(0, "0"); }
				cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
			}
		}

		// ORG instruction.
		if (a_inst.GetOpCode() == "ORG") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }
	}

	if (a_st == Instruction::ST_MachineLanguage) {
		int pointerLoc; // Used to get pointer location from symbol table.
		intToString << a_inst.GetNumOpCode();
		m_strContents = intToString.str();
		
		// HALT OpCode.
		if (a_inst.GetOpCode() == "HALT") {
			for (int i = m_strContents.size(); i < 6; i++) {
				m_strContents.insert(2, "0");
			}
			cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
		}
		// For All Other OpCodes.
		else {	
			a_symtab.LookupSymbol(a_inst.GetOperand(), pointerLoc);

			// Checks for multiply defined symbol.
			if (pointerLoc == -999)	{
				string emsg = "Error: Multiply Defined Symbol";
				Errors::RecordError(emsg);
			}

			// If pointer location cannot be found.
			if (a_symtab.LookupSymbol(a_inst.GetOperand(), pointerLoc) == false) {	
			
				// If OpCode is only 1 char a 0 is added to front.	
				if (m_strContents.size() == 1) { m_strContents.insert(0, "0"); }	

				// ???? added at end of m_strContents to show location cannot be found.
				m_strContents = m_strContents + "????";	
				cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << setw(12) << left << a_inst.GetOriginalInstruction() << endl;

				string emsg = "Error: Undefined label";
				Errors::RecordError(emsg);
			}
			
			if(a_symtab.LookupSymbol(a_inst.GetOperand(), pointerLoc) == true)
			{
				// If OpCode is only 1 char a 0 is added to front
				if (m_strContents.size() == 1) { m_strContents.insert(0, "0"); }	
				intToString.str("");	// Clear stringstream for use again
				intToString << pointerLoc;
				m_strContents = m_strContents + intToString.str();

				// If the m_strContents is size 5, we need to add a 0 after the OpCode.
				if (m_strContents.size() == 5) { m_strContents.insert(2, "0"); }	
				cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << setw(12) << left << a_inst.GetOriginalInstruction() << endl;
			}
		}
	}		
}
