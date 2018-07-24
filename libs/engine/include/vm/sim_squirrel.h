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

#ifndef __SIM_VIRTUAL_MACHINE_H
#define __SIM_VIRTUAL_MACHINE_H

#include <squirrel.h>
#include <sqrat.h>
#include <map>

#include <iostream>
#include <stdarg.h>

#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>
#include <sqratimport.h>
#include <sqrdbg.h>
#include <sqdbgserver.h>

#include <core/sim_core.h>
#include <core/sim_singleton.h>
#include <core/sys/sim_mutex.h>

namespace sim
{
namespace vm
{
// ----------------------------------------------------------------------//
class CScript;
// ----------------------------------------------------------------------//

class CSquirrel : public stl::CSingleton<CSquirrel>
{
public:
    typedef enum 
    {
        k_Error_None, 
		k_Error_Compile, 
		k_Error_Runtime

    } K_ERROR;
	// ------------------------------------------------------------------//
    CSquirrel();
    ~CSquirrel();
	// ------------------------------------------------------------------//
    HSQUIRRELVM						GetVM() { return m_vm; }
	Sqrat::RootTable&				GetRootTable() { return *m_rootTable; }
	Sqrat::ConstTable&				GetConstTable() { return *m_constTable; }

    Sqrat::string					GetLastErrorMsg() { return m_lastErrorMsg; }
    void							SetLastErrorMsg(const Sqrat::string& str) { m_lastErrorMsg = str; }

    void							SetPrintFunc(SQPRINTFUNCTION printFunc, SQPRINTFUNCTION errFunc);
    void							SetErrorHandler(SQFUNCTION runErr, SQCOMPILERERROR comErr);

    K_ERROR							Exec( vm::CScript* script );

	void							DebuggerStart();
	void							DebuggerStop();
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
    static void						PrintFunc(HSQUIRRELVM v, const SQChar *s, ...);
    static SQInteger				RuntimeErrorHandler(HSQUIRRELVM v);
    static void						CompilerErrorHandler(HSQUIRRELVM v,
														const SQChar* desc,
														const SQChar* source,
														SQInteger line,
														SQInteger column);
	// ------------------------------------------------------------------//
protected:
	// ------------------------------------------------------------------//
    HSQUIRRELVM						m_vm;
    Sqrat::RootTable*				m_rootTable;
	Sqrat::ConstTable*				m_constTable;
    static Sqrat::string			m_lastErrorMsg;

	HSQREMOTEDBG					m_debugger;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim

#endif // __SIM_VIRTUAL_MACHINE_H
