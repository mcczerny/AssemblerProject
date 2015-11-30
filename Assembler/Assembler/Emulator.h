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
		if(a_location > 9999){
				return false;
			}
		else	// Adds to memory location
		{
			m_memory[a_location] = a_contents;
			return true;
		}	
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
				
				opCode = stoi(strContents) / 10000;
				address = stoi(strContents) % 10000;

				switch (opCode) {
				default: 
					return false;
					break;
				case 1:
					ACC = ACC + address; // ACC <- c(ACC) + c(ADDR)
					break;
				case 2:
					ACC = ACC - address; // ACC <- c(ACC) - c(ADDR)
					break;
				case 3: 
					ACC = ACC * address; // ACC <- c(ACC) * c(ADDR)
					break;
				case 4: 
					ACC = ACC / address; // ACC <- c(ACC) / c(ADDR)
					break;
				case 5: 
					ACC = m_memory[address]; // ACC <- c(ADDR)
					break;
				case 6: 
					m_memory[address] = ACC; // ADDR <- c(ACC)
					break;
				case 7: 
					cout << "? ";	need to take only 6 digits still			// A line is read and its first 6 digits
					cin >> m_memory[address];	// are placed in the specified address
					break;
				case 8: 
					cout << m_memory[address] << endl; // c(ADDR) is displayed
					break;
				case 9:
					i = address; // go to ADDR for next instruction
					break;
				case 10:
					if (ACC < 0) { i = address; } // go to ADDR if c(ACC) < 0
					break;
				case 11:
					if (ACC == 0) { i = address; } // go to ADDR if c(ACC) = 0
					break;
				case 12: 
					if (ACC > 0) { i = (address-1); } // go to ADDR if c(ACC) > 0
					break;
				case 13: // terminate execution
					return true;
					break;
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
