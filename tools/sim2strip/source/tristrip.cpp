#include <iostream>
#include <vector>
#include <map>

void onStart();

int main(int argc, char** argv)
{
	onStart();

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-t", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			argv[i];
		}
	}

	return EXIT_SUCCESS;
}



// ----------------------------------------------------------------------//

void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 138;
#endif

	atexit(onQuit);
}
