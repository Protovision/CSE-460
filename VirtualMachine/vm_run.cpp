#include "VirtualMachine.h"

void	VirtualMachine::run(int cycles)
{

	int end_time;

	if (running == NULL) {
		clock += cycles;

#if VM_ENABLE_ACCOUNTING == 1
		idle_ticks += cycles;
#endif
		return;
	}

	if (clock > 5 && clock - running->clock < operation_cycles[running->ir.op]) {
		if ( (clock + cycles) - running->clock < operation_cycles[running->ir.op]) {
			clock += cycles;
			running->clock = clock;
			return;
		}
		cycles -= (operation_cycles[running->ir.op] - (clock - running->clock));
	}

	end_time = clock + cycles;

	for (;;) {
			
		if (running->pc < 0 || running->pc >= running->size) {
			running->sr.status = ST_RANGE;
			return;
		}

		*((uint16_t*)&running->ir) = mem[running->base + running->pc];
		
		switch (running->ir.op) {
		case OP_LOAD:
			if (running->ir.i) {
				running->r[running->ir.rd] = running->ir.constant;
			} else {
				if (running->ir.addr >= running->size) {
					running->sr.status = ST_RANGE;
					return;
				}
				running->r[running->ir.rd] = mem[running->base + running->ir.addr];
			}
			break;
		case OP_STORE:
			if (running->ir.addr >= running->size) {
				running->sr.status = ST_RANGE;
				return;
			}
			mem[running->base + running->ir.addr] = running->r[running->ir.rd];
			break;
		case OP_ADD:
			(running->ir.i) ?
			(running->sr.overflow = (USHRT_MAX - running->r[running->ir.rd] < running->ir.constant),
			running->r[running->ir.rd] += running->ir.constant) :
			(running->sr.overflow = (USHRT_MAX - running->r[running->ir.rd] < running->r[running->ir.rs]),
			running->r[running->ir.rd] += running->r[running->ir.rs]);
			running->sr.carry = ((running->r[running->ir.rd] & 1) == 0);
			break;
		case OP_ADDC:
			(running->ir.i) ? 
			(running->sr.overflow = (USHRT_MAX - running->r[running->ir.rd] < running->ir.constant + running->sr.carry),
			running->r[running->ir.rd] += running->ir.constant + running->sr.carry) :
			(running->sr.overflow = (USHRT_MAX - running->r[running->ir.rd] < running->r[running->ir.rs] + running->sr.carry),
			running->r[running->ir.rd] += running->r[running->ir.rs] + running->sr.carry);
			running->sr.carry = ((running->r[running->ir.rd] & 1) == 0);
			break;
		case OP_SUB:
			(running->ir.i) ? 
			(running->sr.overflow = (running->ir.constant > running->r[running->ir.rd]),
			running->r[running->ir.rd] -= running->ir.constant) : 
			(running->sr.overflow = (running->r[running->ir.rs] > running->r[running->ir.rd]),
			running->r[running->ir.rd] -= running->r[running->ir.rs]);
			running->sr.carry = (running->r[running->ir.rd] & 1);
			break;
		case OP_SUBC:
			(running->ir.i) ?
			(running->sr.overflow = ((running->ir.constant + running->sr.carry) > running->r[running->ir.rd]),
			running->r[running->ir.rd] -= running->ir.constant + running->sr.carry) :
			(running->sr.overflow = ((running->r[running->ir.rs] + running->sr.carry) > running->r[running->ir.rd]),
			running->r[running->ir.rd] -= running->r[running->ir.rs] + running->sr.carry);
			running->sr.carry = (running->r[running->ir.rd] & 1);
			break;
		case OP_AND:
			(running->ir.i) ?
			running->r[running->ir.rd] &= running->ir.constant :
			running->r[running->ir.rd] &= running->r[running->ir.rs];
			break;
		case OP_XOR:
			(running->ir.i) ?
			running->r[running->ir.rd] ^= running->ir.constant :
			running->r[running->ir.rd] ^= running->r[running->ir.rs];
			break;
		case OP_COMPL:
			running->r[running->ir.rd] = ~running->r[running->ir.rd];
			break;
		case OP_SHL:
			running->sr.carry = (running->r[running->ir.rd] & 16);
			running->r[running->ir.rd] <<= 1;
			break;
		case OP_SHLA:
			running->sr.carry = (running->r[running->ir.rd] & 16);
			running->r[running->ir.rd] = ((int16_t)running->r[running->ir.rd]) << 1;
			break;
		case OP_SHR:
			running->sr.carry = (running->r[running->ir.rd] & 1);
			running->r[running->ir.rd] >>= 1;
			break;
		case OP_SHRA:
			running->sr.carry = (running->r[running->ir.rd] & 1);
			running->r[running->ir.rd] = ((int16_t)running->r[running->ir.rd]) >> 1;
			break;
		case OP_COMPR:
			if (running->ir.i) {
				running->sr.less = running->sr.equal = running->sr.greater = 0;
				if (running->r[running->ir.rd] < running->ir.constant) {
					running->sr.less = 1;
				} else if (running->r[running->ir.rd] > running->ir.constant) {
					running->sr.greater = 1;
				} else {
					running->sr.equal = 1;
				}
			} else {
				running->sr.less = running->sr.equal = running->sr.greater = 0;
				if (running->r[running->ir.rd] < running->r[running->ir.rs]) {
					running->sr.less = 1;
				} else if (running->r[running->ir.rd] > running->r[running->ir.rs]) {
					running->sr.greater = 1;
				} else {
					running->sr.equal = 1;
				}
			}
			break;
		case OP_GETSTAT:
			running->r[running->ir.rd] = *(uint16_t*)&running->sr;
			break;
		case OP_PUTSTAT:
			*(uint16_t*)&running->sr = running->r[running->ir.rd];
			break;
		case OP_JUMP:
			running->pc = running->ir.addr;
			break;
		case OP_JUMPL:
			if (running->sr.less) {
				running->pc = running->ir.addr;
			} else {
				++running->pc;
			}
			break;
		case OP_JUMPE:
			if (running->sr.equal) {
				running->pc = running->ir.addr;
			} else {
				++running->pc;
			}
			break;
		case OP_JUMPG:
			if (running->sr.greater) {
				running->pc = running->ir.addr;
			} else {
				++running->pc;
			}
			break;
		case OP_CALL:
			if ((running->sp < 6) || (running->sp - 6 <= running->base + running->size)) {
				running->sr.status = ST_OVERFLOW;
				return;
			}
			mem[running->sp--] = ++running->pc;
			mem[running->sp--] = *(uint16_t*)&running->sr;
			mem[running->sp--] = running->r[0];
			mem[running->sp--] = running->r[1];
			mem[running->sp--] = running->r[2];
			mem[running->sp--] = running->r[3];
			running->pc = running->ir.addr;
			break;
		case OP_RETURN:
			if (running->sp + 6 > VM_MEM_SIZE) {
				running->sr.status = ST_UNDERFLOW;
				return;
			}
			running->r[3] = mem[++running->sp];
			running->r[2] = mem[++running->sp];
			running->r[1] = mem[++running->sp];
			running->r[0] = mem[++running->sp];
			*(uint16_t*)&running->sr = mem[++running->sp];
			running->pc = mem[++running->sp];
			break;
		case OP_READ:
			running->input >> running->r[running->ir.rd];
			running->sr.status = ST_READING;
			running->sr.io = running->ir.rd;
			break;
		case OP_WRITE:
			running->output << (int16_t)running->r[running->ir.rd] << std::endl;
			running->sr.status = ST_WRITING;
			running->sr.io = running->ir.rd;
			break;
		case OP_HALT:
			running->sr.status = ST_HALT;
			break;
		case OP_NOOP:
			break;
		default:
			running->sr.status = ST_ILSEQ;
			return;
		}

		if (running->ir.op < OP_JUMP || running->ir.op > OP_RETURN) ++running->pc;

		if (clock + operation_cycles[running->ir.op] >= end_time) {
#if VM_ENABLE_ACCOUNTING == 1
			running->cpu_time += end_time - clock;
#endif
			running->clock = clock = end_time;
			return;
		}
		clock += operation_cycles[running->ir.op];

#if VM_ENABLE_ACCOUNTING == 1
		running->cpu_time += operation_cycles[running->ir.op];
#endif
	
		if (running->sr.status != ST_HALT) {
			running->sr.status = ST_AGAIN;
		} else {
			return;
		}
	}	
}
