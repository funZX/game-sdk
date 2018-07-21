#include <iostream>
#include <vector>
#include <map>

#include <simarian.h>
#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

void onStart();

int main(int argc, char** argv)
{
	onStart();
	
	if( argc != 3 )
	{
		printf( "sq2sim\tinput.nut\toutput.nut" );
		exit( EXIT_SUCCESS );
	}

	std::string inFile(argv[ 1 ] );
	std::string outFile(argv[ 2 ] );
	
	Sqrat::string errorMsg;
	CSquirrel* vm	= SIM_NEW CSquirrel();
	CScript* script = SIM_NEW CScript( "", vm );

	std::cout << "sq2sim: compiling \"" << inFile << "\" to \"" << outFile << "\"";

	script->CompileFile(inFile, errorMsg);
	script->WriteCompiledFile(outFile);

	//script->CompileFile(outFile, errorMsg);
	//vm->Exec(script);

	SIM_SAFE_DELETE( script );
	SIM_SAFE_DELETE( vm );

	if ( !errorMsg.empty() ) 
		std::cout << "sq2sim: error compiling \"" << inFile << "\"";

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