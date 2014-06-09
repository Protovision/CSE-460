/*
 * OperatingSystem.h
 *
 * Author: Mark Swoope
 * Description:
 * When load is called, the operating system will
 * make the assembler assemble all the .s files in the 
 * current directory, then it will load each generated 
 * object file in the virtual machine. The run function 
 * will execute the virtual machine.
 * Each object file is set to have a maximum CPU time 
 * of 15 clock cycles before the next context switch.
 */

#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include "Assembler/Assembler.h"
#include "VirtualMachine/VirtualMachine.h"

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define OS_TIME_SLICE	15

class OperatingSystem {
private:
	Assembler as;
	VirtualMachine vm;
public:
	OperatingSystem() : as(), vm() {}

	void	load();
	void	run();	
};

#endif
