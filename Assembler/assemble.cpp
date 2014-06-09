#include "Assembler.h"

/*
 * Assemble name.s into name.o
 */
bool	Assembler::assemble(const char *name)
{
	int i;
	struct Instruction s;

	if (!inputfile(asmf, name, ".s")) return false;
	if (!outputfile(of, name, ".o")) return false;
	symbols.clear();

	while (parse());
	asmf.clear();
	asmf.seekg(0);
	for (;;) {
		i = generate(&s);
		if (i == 1) {
#if BYTECODE == 1
			of.write((const char *)&s, sizeof(s));
#else
			of << *(uint16_t*)&s << std::endl;
#endif
		} else if (i == -1) break;
		else if (i == -2) {
			of.close();
			return false;
		}
	}

	of.close();
	asmf.close();
	symbols.clear();
	return true;
}
