#include <iostream>

void onStart();

int main(int argc, char** argv)
{
	onStart();
	
	

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