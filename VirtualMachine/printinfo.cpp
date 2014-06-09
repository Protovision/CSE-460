#include "VirtualMachine.h"

void	VirtualMachine::printinfo() const
{
	std::cout <<
		"System time: " << (context_switch_ticks + idle_ticks)/1000.0 << std::endl <<
		"System CPU utilization: " << (double)(context_switch_ticks+idle_ticks)/clock << std::endl <<
		"User CPU utilization: " << (double)jobs_cpu_time/clock << std::endl <<
		"Throughput: " << (clock/1000.0)/(double)total_processes << std::endl;
}
