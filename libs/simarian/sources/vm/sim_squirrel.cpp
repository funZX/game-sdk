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

#include <vm/sim_squirrel.h>
#include <vm/sim_script.h>

namespace sim
{
namespace vm
{
// ----------------------------------------------------------------------//
#ifdef SQUNICODE
    #define scvprintf vwprintf
#else
    #define scvprintf vprintf
#endif

Sqrat::string CSquirrel::m_lastErrorMsg;

CSquirrel::CSquirrel()
    : m_vm( sq_open( 1024 ) )
    , m_rootTable( SIM_NEW Sqrat::RootTable( m_vm ) )
	, m_constTable( SIM_NEW Sqrat::ConstTable( m_vm ) )
{
    //register std libs
    sq_pushroottable( m_vm );
    sqstd_register_iolib( m_vm );
    sqstd_register_bloblib( m_vm );
    sqstd_register_mathlib( m_vm );
    sqstd_register_systemlib( m_vm );
    sqstd_register_stringlib( m_vm );
	sqrat_register_importlib( m_vm );
    sq_pop( m_vm, 1 );

    SetPrintFunc( PrintFunc, PrintFunc );
    SetErrorHandler( RuntimeErrorHandler, CompilerErrorHandler );

	m_debugger = sq_rdbg_init( m_vm, 28000, SQTrue );
	sq_enabledebuginfo( m_vm, SQTrue );
}

// ----------------------------------------------------------------------//

CSquirrel::~CSquirrel()
{
	sq_enabledebuginfo( m_vm, SQFalse );

	SIM_SAFE_DELETE(m_debugger);

	SIM_DELETE( m_constTable );
    SIM_DELETE( m_rootTable );
    sq_close(m_vm);
}

// ----------------------------------------------------------------------//

void CSquirrel::PrintFunc( HSQUIRRELVM v, const SQChar *s, ... )
{
    va_list vl;
    va_start( vl, s );
    scvprintf( s, vl );
    va_end( vl );
}

// ----------------------------------------------------------------------//

SQInteger CSquirrel::RuntimeErrorHandler( HSQUIRRELVM v )
{
    const SQChar *sErr = 0;
    if( sq_gettop(v) >= 1 )
    {
        Sqrat::string& errStr = m_lastErrorMsg;

        if( SQ_SUCCEEDED( sq_getstring( v, 2, &sErr ) ) )
        {
            //scprintf(_SC("RuntimeError: %s\n"), sErr);
            //errStr = _SC("RuntimeError: ") + sErr;
            errStr = sErr;
        }
        else
        {
            //scprintf(_SC("An Unknown RuntimeError Occured.\n"));
            errStr = _SC( "An Unknown RuntimeError Occured." );
        }
    }
    return 0;
}

// ----------------------------------------------------------------------//

void CSquirrel::CompilerErrorHandler( HSQUIRRELVM v,
    const SQChar* desc,
    const SQChar* source,
    SQInteger line,
    SQInteger column )
{
    //scprintf(_SC("%s(%d:%d): %s\n"), source, line, column, desc);

    SQChar buf[ 512 ];
    scsprintf( buf, _SC("%s(%d:%d): %s"), source, line, column, desc );
    m_lastErrorMsg = buf;
}

// ----------------------------------------------------------------------//

void CSquirrel::SetPrintFunc( SQPRINTFUNCTION printFunc, SQPRINTFUNCTION errFunc )
{
    sq_setprintfunc( m_vm, printFunc, errFunc );
}

// ----------------------------------------------------------------------//

void CSquirrel::SetErrorHandler( SQFUNCTION runErr, SQCOMPILERERROR comErr )
{
    sq_newclosure( m_vm, runErr, 0 );
    sq_seterrorhandler( m_vm );

    sq_setcompilererrorhandler( m_vm, comErr );
}

// ----------------------------------------------------------------------//

CSquirrel::K_ERROR CSquirrel::Exec( vm::CScript* script )
{
    Sqrat::string msg;
	return script->Run( msg ) ? k_Error_None : k_Error_Runtime;
}

// ----------------------------------------------------------------------//

void CSquirrel::DebuggerStart()
{
	sq_rdbg_waitforconnections( m_debugger );
}

// ----------------------------------------------------------------------//

void CSquirrel::DebuggerStop()
{
	sq_rdbg_shutdown( m_debugger );
}

// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim
