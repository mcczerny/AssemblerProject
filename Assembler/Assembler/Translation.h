#pragma once
#include "Instruction.h"
#include "SymTab.h"
#include "Emulator.h"

class Translation
{
public:
	void DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, emulator a_emul, int a_loc);
	
	string getStrContents() {
		return m_strContents;
	}
private:
	string m_strContents;
};



