#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
typedef void (*HIFUNC)();
int
main(int argc, char* argv[])
{
	void* library;
	HIFUNC func;
	const char* error;
#ifdef ABSOLUTE_PATH

	library = dlopen("./dlhi.so", RTLD_LAZY);
#else
	library = dlopen("dlhi.so", RTLD_LAZY);
#endif
	if (0 == library)
	{
		fprintf(stderr, "dlopen() - %s\n", dlerror());
		exit(1);
	}
	dlerror();
	func = dlsym(library, "hi");
	error = dlerror();
	if (0 != error)
	{
		fprintf(stderr, "dlsym() - %s\n", error);
		dlclose(library);
		exit(1);
	}

	(*func)();

	dlclose(library);

	return 0;
}
