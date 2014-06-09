/*
 * Assembler.h
 *
 * Author: Mark Swoope
 * Description:
 * Generates a .o file from a .s file 
 * containing the assembly code.
 *
 * Uses a two-pass method to resolve label symbols and 
 * generate the object code.
 */

#ifndef ASM_H
#define ASM_H

#include "ISA/ISA.h"
#include <fstream>
#include <map>
#include <cstring>
#include <cctype>

#ifndef BYTECODE
#define BYTECODE 0
#endif

class Assembler {
protected:
	std::fstream 	asmf;				//Stream to assembly file
	std::fstream	of;				//Stream to object file
	std::map<std::string,uint8_t>	symbols;	//Symbol table

	//Parse next token from assembly file
	bool	parse();

	//Generate next instruction from parsed assembly file
	int	generate(struct Instruction *s);
public:
	Assembler() : asmf(), of(), symbols() {}

	//Assembles name.s into name.o
	bool	assemble(const char *name);
};

#endif
