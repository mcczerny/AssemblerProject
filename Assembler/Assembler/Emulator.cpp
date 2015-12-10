//
//		Implementation of the Emulator class
//
#include "stdafx.h"
#include "Emulator.h"

/**/
/*
Emulator::Emulator() Emulator::Emulator()

NAME

	Emulator::Emulator - Constructor for the emulator

SYNOPSIS

	Emulator::Emulator();

DESCRIPTION

	The constructor will set the blocks of memory from the VC3600 equal to 0.

RETURNS

	Returns no values.

*/
/**/
Emulator::Emulator() {
	{
		memset(m_memory, 0, MEMSZ * sizeof(int));
	}
}

bool 
Emulator::InsertMemory(int a_location, int a_contents)
{
	if (a_location > 9999) {
		return false;
	}
	else {	// Adds to memory location
		m_memory[a_location] = a_contents;
		return true;
	}
}

bool
Emulator::RunProgram()
{
	int ACC = 0;
	for (int i = 100; i < MEMSZ; i++)
		if (m_memory[i] != 0)
		{
			string strContents;
			ostringstream intToString;
			intToString << m_memory[i];
			strContents = intToString.str();

			int opCode = stoi(strContents) / 10000;
			int address = stoi(strContents) % 10000;
			if (address > 9999) {
				cout << "Error: Not a valid memory location" << endl;
				return false;
			}

			switch (opCode) {
			default:
				cout << "Error: Illegal OpCode" << endl;
				return false;
				break;
			case 1:
				ACC = ACC + m_memory[address]; // ACC <- c(ACC) + c(ADDR)
				break;
			case 2:
				ACC = ACC - m_memory[address]; // ACC <- c(ACC) - c(ADDR)
				break;
			case 3:
				ACC = ACC * m_memory[address]; // ACC <- c(ACC) * c(ADDR)
				break;
			case 4:
				ACC = ACC / m_memory[address]; // ACC <- c(ACC) / c(ADDR)
				break;
			case 5:
				ACC = m_memory[address]; // ACC <- c(ADDR)
				break;
			case 6:
				m_memory[address] = ACC; // ADDR <- c(ACC)
				break;
			case 7:
				char userInput[7];
				cout << "? ";
				cin.getline(userInput, 7);	// A line is read and its first 6 digits
				if (cin.fail())				// are placed in the specified address
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				m_memory[address] = stoi(userInput);
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
				if (ACC > 0) { i = (address - 1); } // go to ADDR if c(ACC) > 0
				break;
			case 13: // terminate execution
				return true;
				break;
			}
		}
	return true;
}