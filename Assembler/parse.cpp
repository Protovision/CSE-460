#include "Assembler.h"

/*
 * Parses assembly by determining the address of 
 * each label/symbol. A label is a single line 
 * with a colon at the end.
 *
 * Each call to this function parses a single line. 
 *
 * Returns true if there are more lines to parse
 */
bool	Assembler::parse()
{
	std::string line;
	static uint16_t	addr = 0;

	getline(asmf, line);	
	if (line.size() == 0) {
		if (!asmf.good()) {
			addr = 0;
			return false;
		}
		return true;
	}
	if (line[line.size()-1] == ':') {
		symbols[line.substr(0,line.size()-1)] = addr;
	} else ++addr;
	return asmf.good();
}

