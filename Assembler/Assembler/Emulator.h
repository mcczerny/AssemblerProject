//
//		Emulator class - Supports the emulation of VC3600 programs.
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class Emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	
	// Constructor for the emulator.
	Emulator(); 

	// Records instructions and data into VC3600 memory.
	bool InsertMemory(int a_location, int a_contents); 

	// Runs the VC3600 program recorded in memory.
	bool RunProgram(); 
private:

	// The memory of the VC3600.
	int m_memory[MEMSZ];
};

#endif
#pragma once
