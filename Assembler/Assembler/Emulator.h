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
		
		int ACC = 0;
		for (int i = 0; i < MEMSZ; i++)
			if (m_memory[i] != 0)
			{
				string strContents;
				ostringstream intToString;
				intToString << m_memory[i];
				strContents = intToString.str();
				int opCode;
				int address;
				if (strContents.length() == 5){


					opCode = stoi(strContents.substr(0, 1));
					address = stoi(strContents.substr(1, 5));

				}
				if (strContents.length() == 6) {

					opCode = stoi(strContents.substr(0, 2));
					address = stoi(strContents.substr(2, 5));
				}
				
				switch (opCode) {
				case 1:
					ACC = ACC + address;
					break;
				case 2:
					ACC = ACC - address;
					break;
				case 3: 
					ACC = ACC * address;
					break;
				case 4: 
					ACC = ACC / address;
					break;
				case 5: 
					ACC = address;
					break;
				case 6: 
					address = ACC;
					break;
				case 7: 
					m_memory[i] = m_memory[address];
					break;
				case 8: 
					cout << address;
					break;
				case 9:
					i = address;
					break;
				case 10:
					if (ACC < 0) { i = address; }
					break;
				case 11:
					if (ACC == 0) { i = address; }
					break;
				case 12: 
					if (ACC > 0) { i = address; }
					break;
				case 13: exit(0);
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
