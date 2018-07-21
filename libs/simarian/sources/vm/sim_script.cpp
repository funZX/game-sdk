/* 
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <core/io/sim_mem_stream.h>

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

namespace sim
{
namespace vm
{
CScript::CScript( const std::string &name, CSquirrel* sq )
	:Sqrat::Script( sq->GetVM() )
{
	m_name				= name;
}

// ----------------------------------------------------------------------//

CScript::~CScript()
{

}

// ----------------------------------------------------------------------//

SQInteger CScript::Read(SQUserPointer pObj, SQUserPointer pDest, SQInteger size)
{
	io::CMemStream* memstream = reinterpret_cast<io::CMemStream *>(pObj);

	void* buffer	= memstream->Read(size);

	if( buffer != NULL )
	{
		SIM_MEMCPY(reinterpret_cast<char *>(pDest), buffer, size);

		return size;
	}

	return -1;
}
// ----------------------------------------------------------------------//

SQRESULT CScript::Load( io::CMemStream* ms )
{
	SQRESULT result = sq_readclosure(vm, CScript::Read, (SQUserPointer)(ms));

	sq_getstackobj(vm,-1,&obj);
    sq_addref(vm, &obj);
    sq_pop(vm, 1);

	return result;
}
// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim
