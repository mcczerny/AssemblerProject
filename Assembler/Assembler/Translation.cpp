#include "stdafx.h"
#include "Translation.h"



void Translation::DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, emulator a_emul, int a_loc) {
	ostringstream intToString;	// Used to resize contents to 6 bits
	string m_strContents = "";
	if (a_st == Instruction::ST_AssemblerInstr) {

		if (a_inst.GetOpCode() == "DS") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }

		if (a_inst.GetOpCode() == "DC") {
			intToString << a_inst.GetOperandValue();
			 m_strContents = intToString.str();
			for (int i = m_strContents.size(); i < 6; i++) { m_strContents.insert(0, "0"); }
			cout << setw(12) << left << a_loc << setw(12) << left << m_strContents << a_inst.GetOriginalInstruction() << endl;
			int contents = stoi(m_strContents); // Used to store int version of contents
			a_emul.insertMemory(a_loc, contents); // Inserts into memory
		}
		if (a_inst.GetOpCode() == "ORG") { cout << setw(24) << left << a_loc << a_inst.GetOriginalInstruction() << endl; }

		if (a_inst.GetOpCode() == "END") { cout << "                        " << a_inst.GetOriginalInstruction() << endl; }
	}
	// MACHINE INSTRUCTIONS ARE ANY OP CODE 
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
