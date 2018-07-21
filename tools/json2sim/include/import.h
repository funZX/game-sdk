#ifndef __SIM_IMPORT_H
#define __SIM_IMPORT_H

#include <simarian.h>

#include "data.h"

class CImport
{
	
public:
	CImport( const std::string& file, CData::K_DATA_TYPE type );
	virtual ~CImport();

	bool					Parse( CData* data );

protected:
	bool					ParseMaterial( CData* data );
	bool					ParseMesh( CData* data );
	bool					ParseMeshLOD( CData* data );
	bool					ParseActor( CData* data );
	bool					ParseLight( CData* data );
	bool					ParseCamera( CData* data );
	bool					ParseScene( CData* data );

protected:
	CJsonStream*			m_jsonStream;

	CData::K_DATA_TYPE		m_type;
};

#endif // __SIM_IMPORT_H