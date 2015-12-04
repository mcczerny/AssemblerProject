//
//		Implementation of the translation class.
//
#include "stdafx.h"
#include "Translation.h"

/*
NAME

DisplayTranslation - translates machine and assembly language code and outputs it to console.

SYNOPSIS

void DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, int a_loc)

DESCRIPTION

This function will output the translation, and the original instruction by using "a_st" to determine if instruction is either 
assembly or machine language code. Then "a_inst" is used in order to get the OpCode in order to determine what to output. The 
symbol table will be accessed by "a_symtab" and the "contents" that will be inserted into emulator are generated if needed. 
Finally "a_loc" the generated contents (if needed) and the original instruction are formated is printed out to user.
*/

void Translation::DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, int a_loc) {

	ostringstream intToString;	// Used to resize contents to 6 bits
	if (a_st == Instruction::ST_AssemblerInstr) {

		if (a_inst.GetOpCode() == "DS") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }

		if (a_inst.GetOpCode() == "DC") {				ostringstream intToString;
			intToString << a_inst.GetOperandValue();
			m_strContents = intToString.str();
			for (int i = m_strContents.size(); i < 6; i++) { m_strContents.insert(0, "0"); }
			cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
			}
		
			if (a_inst.GetOpCode() == "ORG") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }

			if (a_inst.GetOpCode() == "END") { cout << "                        " << a_inst.GetOriginalInstruction() << endl; }
	}

	if (a_st == Instruction::ST_MachineLanguage) {
		int pointerLoc; // Used to get pointer location from symbol table
		intToString << a_inst.GetNumOpCode();
		m_strContents = intToString.str();
	
		if (a_inst.GetOpCode() == "HALT") {
			for (int i = m_strContents.size(); i < 6; i++) {
				m_strContents.insert(2, "0");
			}
			cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
		}
		else {	// For All Other OpCodes
			a_symtab.LookupSymbol(a_inst.GetOperand(), pointerLoc);

			if (m_strContents.size() == 1) { m_strContents.insert(0, "0"); }	// If OpCode is only 1 char a 0 is added to front
			intToString.str("");	// Clear stringstream for use again
			intToString << pointerLoc;
			m_strContents = m_strContents + intToString.str();

			if (m_strContents.size() == 5) { m_strContents.insert(2, "0"); }	// If the m_strContents is size 5, we need to add a 0 after the OpCode.
			cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << setw(12) << left << a_inst.GetOriginalInstruction() << endl;
		}
	}		
}
