
#include "data.h"
#include "import.h"
#include "export.h"

#include <map>

void onStart();

int main(int argc, char* argv[])
{
	if( argc != 4 )
	{
		printf( "json2sim\tdata\tinput.json\toutput.sim" );
		printf( "\n data: material, mesh, lod, actor, light, camera, scene" );
		exit( EXIT_SUCCESS );
	}

	onStart();

	std::string input(argv[ 2 ] );
	std::string output(argv[ 3 ] );

	std::map<std::string, CData::K_DATA_TYPE> ops;
	ops["material"]		= CData::k_Data_Type_Material;
	ops["mesh"]			= CData::k_Data_Type_Mesh;
	ops["actor"]		= CData::k_Data_Type_Actor;
	ops["light"]		= CData::k_Data_Type_Light;
	ops["camera"]		= CData::k_Data_Type_Camera;
	ops["scene"]		= CData::k_Data_Type_Scene;

	CData	D;
	bool success = false;

	CImport* I = SIM_NEW CImport( input, ops[ argv[ 1 ] ] );
	success =  I->Parse( &D );

	if ( success )
	{
		CExport* E = SIM_NEW CExport( output );
		success = E->Save( &D ); 

		SIM_SAFE_DELETE( E );
	}

	SIM_SAFE_DELETE( I );

	if (!success)
		SIM_ERROR("ERROR: json2sim %s %s %s", argv[1], argv[2], argv[3]);

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
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 64635;
#endif

	atexit(onQuit);
}

