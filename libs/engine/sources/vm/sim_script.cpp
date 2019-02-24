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

#include <core/io/sim_mem_stream.h>

#include <vm/sim_script.h>
#include <vm/sim_squirrel.h>

namespace sim
{
namespace vm
{
// ----------------------------------------------------------------------//
	
CScript::CScript(CSquirrel* sq)
	:Sqrat::Script( sq->GetSQVM() )
{
}

// ----------------------------------------------------------------------//

CScript::CScript( const std::string &name, CSquirrel* sq )
	: CScript(sq)
{
	m_name = name;
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

	if( buffer != nullptr )
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
