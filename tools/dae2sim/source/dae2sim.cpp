#include <iostream>
#include <dae.h>

void onStart();

int main(int argc, char** argv)
{
	onStart();
	
	DAE dae;
	daeElement* root = dae.open("simpleImport.dae");
	
	if (!root)
	{
		std::cout << "Document import failed.\n";
		return 0;
	}

	return 0;
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
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 138;
#endif

	atexit(onQuit);
}