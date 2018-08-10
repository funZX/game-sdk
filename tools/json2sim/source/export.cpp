#include "data.h"

#include "export.h"

CExport :: CExport( const std::string& file )
{
	m_filename = file;
}

CExport :: ~CExport()
{

}

bool CExport::Save( CData* data )
{
	FILE* file = fopen( m_filename.c_str(), "wb" );

	if ( !file )
		return false;

	if ( data->m_material != NULL )
		return SaveMaterial( data, file );

	if ( data->m_mesh != NULL )
		return SaveMesh( data, file );

	if ( data->m_actor != NULL )
		return SaveActor( data, file );

	if ( data->m_light != NULL )
		return SaveLight( data, file );

	if ( data->m_camera != NULL )
		return SaveCamera( data, file );

	if ( data->m_scene != NULL )
		return SaveScene( data, file );

	fclose( file );

	return false;
}
