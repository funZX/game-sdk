/*
*
*    MIT License
*
*    SIM - OpenGL ES 2.0 portable game engine
*    Copyright (C) 2012  Adrian SIMINCIUC
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
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
    HSQUIRRELVM						GetSQVM() { return m_vm; }
	Sqrat::RootTable&				GetRootTable() { return *m_rootTable; }
	Sqrat::ConstTable&				GetConstTable() { return *m_constTable; }

    Sqrat::string					GetLastErrorMsg() { return m_lastErrorMsg; }
    void							SetLastErrorMsg(const Sqrat::string& str) { m_lastErrorMsg = str; }

    void							SetPrintFunc(SQPRINTFUNCTION printFunc, SQPRINTFUNCTION errFunc);
    void							SetErrorHandler(SQFUNCTION runErr, SQCOMPILERERROR comErr);

    K_ERROR							Exec( vm::CScript* script );

#if SIM_DEBUG
	void							DebuggerStart();
	void							DebuggerStop();
#endif // SIM_DEBUG
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

#if SIM_DEBUG
	bool							m_isDebuggerEnabled;
	HSQREMOTEDBG					m_debugger;
#endif // SIM_DEBUG
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim

#endif // __SIM_VIRTUAL_MACHINE_H
