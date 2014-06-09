#include "Assembler.h"

/*
 * Generates the next machine code instruction.
 * This function is called after the assembly file 
 * is fully parsed.
 *
 * Returns -1 if at end of file, 0 if the current line 
 * is a label (no instruction generated), and 1 if
 * an instruction was generated
 */
int	Assembler::generate(struct Instruction *s)
{
	std::string token;
	struct Operator *op;
	std::map<std::string,uint8_t>::iterator it;

	*(uint16_t*)s = 0;

	asmf >> token;
	if (token.size() == 0) return -1;
	if (token[token.size()-1] == ':') return 0;
	if (token[0] == '!') {
		getline(asmf, token);
		return 0;
	}

	//*(uint16_t*)s = 0;

	op = find_operation(token.c_str());
	if (op == NULL) return -1;

	if (op->i == 1) s->i = 1;
	else s->i = 0;
	
	s->op = op->op;
	if (op->operands & OPERAND_RD) {
		asmf >> token;
		if (token.size() == 0) return -2;
		if (!isdigit(token[0])) return -2;
		s->rd = atoi(token.c_str());
	}
	if (op->operands & OPERAND_RS) {
		asmf >> token;
		if (token.size() == 0) return -2;
		if (!isdigit(token[0])) return -2;
		s->rs = atoi(token.c_str());
	}
	if (op->operands & OPERAND_ADDR) {
		asmf >> token;
		if (token.size() == 0) return -2;
		if (isalpha(token[0])) {
			it = symbols.find(token);
			if (it == symbols.end()) return -2;
			s->addr = it->second;
		} else {
			s->addr = (uint8_t)atoi(token.c_str());
		}
	} else if (op->operands & OPERAND_CONST) {
		asmf >> token;
		if (token.size() == 0) return -2;
		if (!isdigit(token[0]) && token[0] != '-' && token[0] != '+') return -2;
		s->constant = (int8_t)atoi(token.c_str());
	}
	return 1;
}

