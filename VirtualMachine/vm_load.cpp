#include "VirtualMachine.h"

bool	VirtualMachine::load(const char *name)
{
	size_t size;
	std::fstream infile;
	struct ProgramControlBlock *pcb;

	if (!inputfile(infile, name, ".o")) return false;
	pcb = new struct ProgramControlBlock;

#if BYTECODE == 1
	infile.read((char*)(mem+ip), (VM_MEM_SIZE-ip)*sizeof(uint16_t));
	pcb->size = ifs.gcount()/sizeof(uint16_t);
#else
	for (size = 0; ; ++size) {
		if (!infile.good()) break;
		infile >> mem[ip + size];
	}
	pcb->size = size;
#endif

	*(uint16_t*)&pcb->ir = 0;
	*(uint16_t*)&pcb->sr = 0;
	pcb->r[0] = 0;
	pcb->r[1] = 0;
	pcb->r[2] = 0;
	pcb->r[3] = 0;
	pcb->pc = 0;
	pcb->sp = VM_MEM_SIZE;
	pcb->clock = clock;
	pcb->base = ip;
	strcpy(pcb->name, name);
	inputfile(pcb->input, name, ".in");
	outputfile(pcb->output, name, ".out");
	outputfile(pcb->stack, name, ".st");

#if VM_ENABLE_ACCOUNTING == 1
	pcb->cpu_time = 0;
	pcb->waiting_time = 0;
	pcb->io_time = 0;
	pcb->largest_stack_size = 0;

	++total_processes;
#endif

	ip += pcb->size;
	ready.push(pcb);
}
