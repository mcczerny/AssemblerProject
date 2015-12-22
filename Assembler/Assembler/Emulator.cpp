//
//		Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"

/**/
/*
Emulator::Emulator() Emulator::Emulator()

NAME

	Emulator::Emulator - Constructor for the emulator.

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

/**/
/*
Emulator::InsertMemory() Emulator::InsertMemory()

NAME

	Emulator::InsertMemory - Records instructions and data into VC3600 Memory.

SYNOPSIS

	bool Emulator::InsertMemory(int a_location, int a_contents);

		a_location	--> The location that in memory to input the contents.
		a_contents	--> The contents to be input into the location.

DESCRIPTION

	This function will insert a_contents into m_memory. The memory location that a_contents is put
	in is a_location. If a_location > 9999 then it is not inserted.

RETURNS

	Returns true if a_contents can be put into m_memory[a_location]. False is returned when a_location
	is greater than 9999 (not enough memory).

*/
/**/
bool 
Emulator::InsertMemory(int a_location, int a_contents)
{
	// VC3600 does not have more than 10000 memory locations. Cannot insert.
	if (a_location > 9999) {
		return false;
	}
	// Adds to memory location.
	else {	
		m_memory[a_location] = a_contents;
		return true;
	}
}

/**/
/*
Emulator::RunProgram() Emulator::RunProgram()

NAME

	Emulator::RunProgram - Runs the VC3600 program recorded in memory.

SYNOPSIS

	bool Emulator::RunProgram();

DESCRIPTION
	
	This function will run the program that is in the VC3600 memory. It will start at location 100
	and work it's way through memory running the machine language stored in each memory location.

RETURNS

	Returns true if program runs sucessfully. Returns false if an error occurs while running.

*/
/**/
bool
Emulator::RunProgram()
{
	int ACC = 0;	// Accumulator.
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

			// If number is to big to fit into memory
			if (ACC > 999999) {
				ACC = ACC % 1000000; 
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
				if (cin.fail())				// are placed in the specified address.
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				m_memory[address] = stoi(userInput);
				break;
			case 8:
				cout << m_memory[address] << endl; // c(ADDR) is displayed.
				break;
			case 9:
				i = address; // go to ADDR for next instruction.
				break;
			case 10:
				if (ACC < 0) { i = address; } // Go to ADDR if c(ACC) < 0.
				break;
			case 11:
				if (ACC == 0) { i = address; } // Go to ADDR if c(ACC) = 0.
				break;
			case 12:
				if (ACC > 0) { i = (address - 1); } // Go to ADDR if c(ACC) > 0.
				break;
			case 13: // Terminate execution.
				return true;
				break;
			}
		}
	return true;
}