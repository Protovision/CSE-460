#include "OperatingSystem.h"

void	OperatingSystem::run()
{
	do {
		vm.swap();
		vm.run(15);
	} while (vm.active());

	vm.printinfo();
}
