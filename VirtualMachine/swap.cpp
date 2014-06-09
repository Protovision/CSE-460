#include "VirtualMachine.h"

void	VirtualMachine::swap()
{
	uint16_t s;
	struct ProgramControlBlock *pcb;

	while (waiting.size()) {
		pcb = waiting.front();

		if (clock - pcb->clock < 28) break;
		ready.push(pcb);
		waiting.pop();
#if VM_ENABLE_ACCOUNTING == 1
		pcb->io_time += (clock - pcb->clock);
#endif
	}

_swap_end:
	if (running != NULL) {
		running->stack.seekp(0);
		for (s = running->sp; s != VM_MEM_SIZE; ++s) {
			running->stack << mem[s] << std::endl;
		}

#if VM_ENABLE_ACCOUNTING == 1
		if (VM_MEM_SIZE - running->sp > running->largest_stack_size) {
			running->largest_stack_size = VM_MEM_SIZE - running->sp;
		}
#endif
		if (running->sr.status == ST_AGAIN) {
			ready.push(running);
		} else if (running->sr.status == ST_READING || running->sr.status == ST_WRITING) {
			waiting.push(running);
		} else {
			switch (running->sr.status) {
			case ST_HALT:
			#if VM_ENABLE_ACCOUNTING == 1
				running->output << std::endl <<
					"CPU time: " << pcb->cpu_time << std::endl <<
					"Waiting time: " << pcb->waiting_time << std::endl <<
					"Turnaround time: " << (pcb->cpu_time + pcb->waiting_time + pcb->io_time)/1000.0 << std::endl <<
					"I/O time: " << pcb->io_time << std::endl <<
					"Largest stack size: " << pcb->largest_stack_size << std::endl;

				jobs_cpu_time += pcb->cpu_time;
					
			#endif
				break;
			case ST_RANGE:
				running->output << "Error: Out of range" << std::endl;
				break;
			case ST_OVERFLOW:
				running->output << "Error: Stack overflow" << std::endl;
				break;
			case ST_UNDERFLOW:
				running->output << "Error: Stack underflow" << std::endl;
				break;
			case ST_ILSEQ:
				running->output << "Error: Illegal instruction" << std::endl;
				break;
			}
			running->input.close();
			running->output.close();
			running->stack.close();
			delete running;	
		}
	}

	if (ready.size() == 0) {
		running = NULL;
	} else {
		running = ready.front();
		ready.pop();

		running->stack.seekg(0);
		for (s = running->sp; s != VM_MEM_SIZE; ++s) {
			if (!running->stack.good()) break;
			running->stack >> mem[s];
		}

#if VM_ENABLE_ACCOUNTING == 1
		running->waiting_time += (clock - running->clock);
#endif
	}
	clock += 5;

#if VM_ENABLE_ACCOUNTING == 1
	context_switch_ticks += 5;
#endif
}
