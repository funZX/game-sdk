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

	ExportSymbols();

#if SIM_DEBUG
	m_rdbg = nullptr;
	DebuggerStart();
#endif // SIM_DEBUG
}

// ----------------------------------------------------------------------//

CSquirrel::~CSquirrel()
{
#if SIM_DEBUG
	if (m_rdbg)
		DebuggerStop();
#endif // SIM_DEBUG

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
#if SIM_DEBUG
void CSquirrel::DebuggerStart()
{
	if (!m_rdbg)
	{
		m_rdbg = sq_rdbg_init(m_vm, 20900, SQTrue);

		sq_enabledebuginfo(m_vm, SQTrue);

		auto start_task = async::spawn([&, this] {
			sq_rdbg_waitforconnections(m_rdbg);
		});

		auto stop_task = start_task.then([&, this] {

			while (!m_rdbg->_terminate)
				sq_rdbg_update(m_rdbg);

			sq_rdbg_shutdown(m_rdbg);

			m_rdbg = nullptr;
			DebuggerStart();
		});
	}
}

void CSquirrel::DebuggerStop()
{
	if (m_rdbg)
	{
		sq_rdbg_term(m_rdbg);
		sq_enabledebuginfo(m_vm, SQFalse);
	}
}
#endif // SIM_DEBUG

// ----------------------------------------------------------------------//
}; // namespace vm
}; // namespace sim
