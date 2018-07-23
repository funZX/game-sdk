#ifndef __SIM_DATA_H
#define __SIM_DATA_H

#include <engine.h>

class CData
{
// ----------------------------------------------------------------------//
	friend class CImport;
	friend class CExport;
	friend class CLodder;
// ----------------------------------------------------------------------//


// ----------------------------------------------------------------------//

public:

	typedef enum
	{
		k_Data_Type_Material,
		k_Data_Type_Mesh,
		k_Data_Type_MeshLOD,
		k_Data_Type_Actor,
		k_Data_Type_Light,
		k_Data_Type_Camera,
		k_Data_Type_Scene,

	} K_DATA_TYPE;

	typedef struct 
	{
		std::string material;

	} TInfo;

// ----------------------------------------------------------------------//

	CData();
	virtual ~CData();

	void						Load(const CImport* i);
	void						Save(const CExport* e);

protected:
	
	CMaterial*					m_material;
	CMesh*						m_mesh;
	CActor*						m_actor;
	CLight*						m_light;
	CCamera*					m_camera;
	CScene*						m_scene;

	TInfo*						m_info;
	CLodder*					m_lodder;
};

#endif // __SIM_DATA_H