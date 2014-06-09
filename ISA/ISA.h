#ifndef ISA_H
#define ISA_H

#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdint>

struct __attribute__ ((__packed__)) Instruction {
	unsigned int	op:5;				//Opcode
	unsigned int	rd:2;				//Destination register

	unsigned int	i:1;				//1 if using immediate value
	union __attribute__ ((__packed__)) {
		struct __attribute__ ((__packed__)) {
			unsigned int	rs:2;		//Source register
			unsigned int	unused1:6;
		};
		struct __attribute__ ((__packed__)) {
			unsigned int	addr:8;		//Address
		};
		struct __attribute__ ((__packed__)) {
			signed int	constant:8;	//Immediate value
		};
	};
};

//Opcodes
enum {
	OP_LOAD,
	OP_STORE,
	OP_ADD,
	OP_ADDC,
	OP_SUB,
	OP_SUBC,
	OP_AND,
	OP_XOR,
	OP_COMPL,
	OP_SHL,
	OP_SHLA,
	OP_SHR,
	OP_SHRA,
	OP_COMPR,
	OP_GETSTAT,
	OP_PUTSTAT,
	OP_JUMP,
	OP_JUMPL,
	OP_JUMPE,
	OP_JUMPG,
	OP_CALL,
	OP_RETURN,
	OP_READ,
	OP_WRITE,
	OP_HALT,
	OP_NOOP
};

//Operand flags
enum {
	OPERAND_RD	= 1,
	OPERAND_RS	= 2,
	OPERAND_ADDR	= 4,
	OPERAND_CONST	= 8
};

//Instruction entry
struct Operator {
	const char	*name;
	int		op;
	int		i;
	int		operands;
	int		cycles;
};

extern struct Operator operations[];
extern int operation_cycles[];

//Search for instruction entry from name
struct Operator *find_operation(const char *name);

int	strtailcmp(const char *s, const char *t);
bool	inputfile(std::fstream &ifs, const char *name, const char *extension);
bool	outputfile(std::fstream &ofs, const char *name, const char *extension, bool truncate = true);

#endif
