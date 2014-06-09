#include "ISA.h"

struct Operator operations[] = {

/*	Name		Op (operation)	I 	Operand flags 			Clock ticks */
	{ "load",	OP_LOAD,	0,	OPERAND_RD|OPERAND_ADDR,	4 },
	{ "loadi",	OP_LOAD,	1,	OPERAND_RD|OPERAND_CONST,	4 },
	{ "store",	OP_STORE,	2,	OPERAND_RD|OPERAND_ADDR,	4 },
	{ "add",	OP_ADD, 	0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "addi",	OP_ADD,		1,	OPERAND_RD|OPERAND_CONST, 	1 },
	{ "addc",	OP_ADDC,	0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "addci",	OP_ADDC,	1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "sub",	OP_SUB,		0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "subi",	OP_SUB,		1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "subc",	OP_SUBC,	0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "subci",	OP_SUBC,	1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "and",	OP_AND,		0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "andi",	OP_AND,		1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "xor",	OP_XOR,		0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "xori",	OP_XOR,		1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "compl",	OP_COMPL,	2,	OPERAND_RD,			1 },
	{ "shl",	OP_SHL,		2,	OPERAND_RD,			1 },
	{ "shla",	OP_SHLA,	2,	OPERAND_RD,			1 },
	{ "shr",	OP_SHR,		2,	OPERAND_RD,			1 },
	{ "shra",	OP_SHRA,	2,	OPERAND_RD,			1 },
	{ "compr",	OP_COMPR,	0,	OPERAND_RD|OPERAND_RS,		1 },
	{ "compri",	OP_COMPR,	1,	OPERAND_RD|OPERAND_CONST,	1 },
	{ "getstat",	OP_GETSTAT,	2,	OPERAND_RD,			1 },
	{ "putstat",	OP_PUTSTAT,	2,	OPERAND_RD,			1 },
	{ "jump",	OP_JUMP,	2,	OPERAND_ADDR,			1 },
	{ "jumpl",	OP_JUMPL,	2,	OPERAND_ADDR,			1 },
	{ "jumpe",	OP_JUMPE,	2,	OPERAND_ADDR,			1 },
	{ "jumpg",	OP_JUMPG,	2,	OPERAND_ADDR,			1 },
	{ "call",	OP_CALL,	2,	OPERAND_ADDR,			4 },
	{ "return",	OP_RETURN,	2,	0,				4 },
	{ "read",	OP_READ,	2,	OPERAND_RD,			28 },
	{ "write",	OP_WRITE,	2,	OPERAND_RD,			28 },
	{ "halt",	OP_HALT,	2,	0,				1 },
	{ "noop",	OP_NOOP,	2,	0,				1 },
	{ NULL,		0,		0,	0,				0 }
};

int operation_cycles[] = {
	4,	//OP_LOAD
	4,	//OP_STORE
	1,	//OP_ADD
	1,	//OP_ADDC
	1,	//OP_SUB
	1,	//OP_SUBC
	1,	//OP_AND
	1,	//OP_XOR
	1,	//OP_COMPL
	1,	//OP_SHL
	1,	//OP_SHLA
	1,	//OP_SHR
	1,	//OP_SHRA
	1,	//OP_COMPR
	1,	//OP_GETSTAT
	1,	//OP_PUTSTAT
	1,	//OP_JUMP
	1,	//OP_JUMPL
	1,	//OP_JUMPE
	1,	//OP_JUMPG
	4,	//OP_CALL
	4,	//OP_RETURN
	28,	//OP_READ
	28,	//OP_WRITE
	1,	//OP_HALT
	1	//OP_NOOP	
};

struct Operator *find_operation(const char *name)
{
	struct Operator *op;
	for (op = operations; op->name != NULL; ++op) {
		if (strcmp(name, op->name) == 0) return op;
	}
	return NULL;
}

int	strtailcmp(const char *s, const char *t)
{
	size_t slen, tlen;
	slen = strlen(s);
	tlen = strlen(t);
	if (slen <= tlen) return strcmp(s, t);
	return strcmp(s + strlen(s) - strlen(t), t);
}

bool	inputfile(std::fstream &ifs, const char *name, const char *extension)
{
	if (ifs.is_open()) ifs.close();
	ifs.open(std::string(name)+extension, std::fstream::in | std::fstream::binary);	
	return ifs.is_open();	
}

bool	outputfile(std::fstream &ofs, const char *name, const char *extension, bool truncate)
{
	std::fstream::openmode mode;

	mode = std::fstream::in | std::fstream::out | std::fstream::binary;
	if (truncate) mode |= std::fstream::trunc;

	if (ofs.is_open()) ofs.close();
	ofs.open(std::string(name)+extension, mode);
	return ofs.is_open();
}
