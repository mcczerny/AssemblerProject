//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"
#include "Errors.h"

/**/
/*
SymbolTable::AddSymbol() SymbolTable::AddSymbol()

NAME

	SymbolTable::AddSymbol - Adds a new symbol to the symbol table.

SYNOPSIS

	void SymbolTablle::AddSymbol( char *a_symbol, int a_loc );

		a_symbol --> The symbol to be placed into symbol table.
		a_loc	 --> The location of the symbol.

DESCRIPTION

	This function will place the symbol "a_symbol" and its location "a_loc" in the symbol table.

RETURNS

	Returns no values.

*/
/**/
void
SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {
		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record a the location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

/**/
/*
SymbolTable::DisplaySymbolTable() SymbolTable::DisplaySymbolTable()

NAME

	SymbolTable::DisplaySymbolTable - Displays the symbol table.

SYNOPSIS

	void SymbolTable::DisplaySymbolTable();

DESCRIPTION

	This function will use the iterator it, to go through map and output each symbol with it's
	corresponding location in the symbol table.

RETURNS

	Returns no values.

*/
/**/
void
SymbolTable::DisplaySymbolTable()
{
	map<string, int>::iterator it;
	int i = 0;
	for (auto it = m_symbolTable.begin(); it != m_symbolTable.end(); it++){
		cout << setw(12) << left << i << setw(12) << left << it->first << setw(12) << left << it->second << endl;
		i++;
	}
}

/**/
/*
SymbolTable::LookupSymbol() SymbolTable::LookupSymbol()

NAME

	SymbolTable::LookupSymbol - Looks for symbol in symbol table. 

SYNOPSIS

	bool LookupSymbol( char *a_symbol, int a_loc );

		a_symbol --> The symbol to be looked up.
		a_loc	 --> The location of the symbol.

DESCRIPTION

	This function will search for symbol in symbol table and replace the symbol a_symbol and its
	location a_loc with the found values in the symbol table. 

RETURNS

	Returns no values.

*/
/**/
bool
SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
{
	map<string, int>::iterator it;

	it = m_symbolTable.find(a_symbol);
	if (it != m_symbolTable.end()) {
		a_symbol = it->first;
		a_loc = it->second;
		return true;
	}
	else
		return false;
	
}
