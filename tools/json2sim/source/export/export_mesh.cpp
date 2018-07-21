#include <core/io/sim_mem_stream.h>

#include <render/sim_vertex_source.h>
#include <render/sim_vertex_group.h>

#include <render/sim_mesh.h>

#include "data.h"
#include "export.h"

// ----------------------------------------------------------------------//

bool CExport::SaveMesh( CData* data, FILE *file )
{
	CMesh* mesh = data->m_mesh;

	CMemStream ms(1024*1024);

	mesh->Save( &ms );

	u32 memsize = 
	ms.GetCursor();
	ms.Rewind();

	fwrite( ms.Read(0), memsize, 1, file );	

	return true;
}


bool CExport::SaveMeshLOD( CData* data, FILE *file )
{


	return true;
}

// ----------------------------------------------------------------------//