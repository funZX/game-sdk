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

#ifndef __SIM_SCRIPT_H
#define __SIM_SCRIPT_H


#include <core/sim_core.h>

#include <core/sim_interfaces.h>
#include <squirrel.h>
#include <sqrat.h>


namespace sim
{
// ----------------------------------------------------------------------//
namespace io { class CMemStream; };
// ----------------------------------------------------------------------//
namespace vm
{
// ----------------------------------------------------------------------//
class CSquirrel;
// ----------------------------------------------------------------------//

class CScript : public Sqrat::Script, public IEngineItem
{
public:
	CScript( CSquirrel* sq );
    CScript( const std::string &name, CSquirrel* sq );
    ~CScript();

	SQRESULT						Load( io::CMemStream* ms );

protected:
	static SQInteger				Read(SQUserPointer pMemObject, SQUserPointer pDest, SQInteger size);
};

// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim

#endif // __SIM_SCRIPT_H
