#include "VirtualMachine.h"

VirtualMachine::~VirtualMachine()
{
	struct ProgramControlBlock *pcb;

	if (running != NULL) {
		ready.push(running);
	}
	while (ready.size()) {
		pcb = ready.front();
		pcb->input.close();
		pcb->output.close();
		pcb->stack.close();
		delete pcb;
		ready.pop();
	}

	while (waiting.size()) {
		pcb = waiting.front();
		pcb->input.close();
		pcb->output.close();
		pcb->stack.close();
		delete pcb;
		waiting.pop();
	}
}

