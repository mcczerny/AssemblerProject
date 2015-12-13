#pragma once
#include "Instruction.h"
#include "SymTab.h"
#include "Emulator.h"

class Translation
{
public:
	// Translates machine and assembly language code and outputs it to console.
	void DisplayTranslation(Instruction::InstructionType a_st, Instruction a_inst, SymbolTable a_symtab, int a_loc);
	
	// To access m_strContents.
	string getStrContents() {
		return m_strContents;
	}
private:
	string m_strContents;	// The string version of "contents" that will be inserted into emulator memory.
};



