//
//		Assembler class - This is a container for all the components that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "Translation.h"



class Assembler {

public:
	// Constructor for the assembler.
	Assembler(int argc, char *argv[]);

	// Pass I - Establishes the locations of the symbols.
	void PassI();
	
	// Displays the symbols in the symbol table.
	void DisplaySymbolTable();

	// Pass II - Generate a translation.
	void PassII();
	
	// Runs the emulator on the translation.
	void RunEmulator();

private:

	FileAccess m_facc;	    // File Access object
	SymbolTable m_symtab;	// Symbol table object
	Instruction m_inst;	    // Instruction object
	Emulator m_emul;        // Emulator for VC3600
	Translation m_trans;	// Translation object for Machine and Assembly code
	
};
