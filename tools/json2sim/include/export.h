#ifndef __SIM_EXPORT_H
#define __SIM_EXPORT_H

#include <simarian.h>

#include "data.h"

class CExport
{
	
public:
	CExport( const std::string& file );
	virtual ~CExport();

	bool					Save( CData* data );

protected:
	bool					SaveMaterial( CData* data, FILE *file );
	bool					SaveMesh( CData* data, FILE *file );
	bool					SaveMeshLOD( CData* data, FILE *file );
	bool					SaveActor( CData* data, FILE *file );
	bool					SaveLight( CData* data, FILE *file );
	bool					SaveCamera( CData* data, FILE *file );
	bool					SaveScene( CData* data, FILE *file );

protected:

	std::string				m_filename;
};

#endif // __SIM_EXPORT_H
