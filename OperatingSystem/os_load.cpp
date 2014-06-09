#include "OperatingSystem.h"

void	OperatingSystem::load()
{
	DIR *dir;
	struct dirent *ent;
	static char buf[256];

	dir = opendir(".");
	if (dir == NULL) return;
	
	for (ent = readdir(dir); ent != NULL; ent = readdir(dir)) {
		if (strtailcmp(ent->d_name, ".s") == 0) {
			*strrchr(ent->d_name, '.') = 0;
			as.assemble(ent->d_name);
			vm.load(ent->d_name);
		}	
	}

	closedir(dir);
}
