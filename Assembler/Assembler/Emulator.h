//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	emulator() {

		memset(m_memory, 0, MEMSZ * sizeof(int));
	}
	// Records instructions and data into VC3600 memory.
	bool insertMemory(int a_location, int a_contents) {
		m_memory[a_location] = a_contents;	
		return true;
	}

	// Runs the VC3600 program recorded in memory.
	bool runProgram() {
		
		int accumulator;
		for (int i = 0; i < MEMSZ; i++)
			if (m_memory[i] != 0)
			{
				string strContents;
				ostringstream intToString;
				intToString << m_memory[i];
				strContents = intToString.str();

				if (strContents.length() == 5){


					int opCode = stoi(strContents.substr(0, 1));
					int address = stoi(strContents.substr(1, 5));

				}
				if (strContents.length() == 6) {

					int opCode = stoi(strContents.substr(0, 2));
					int address = stoi(strContents.substr(2, 5));
				}
				
			}
		return true;
	}

private:

	// The memory of the VC3600.
	int m_memory[MEMSZ];
};

#endif
#pragma once
