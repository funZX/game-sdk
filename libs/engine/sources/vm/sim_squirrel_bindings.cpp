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

#include <sim_engine.h>
#include <vm/sim_squirrel.h>

#include <core/io/sim_file_system.h>

namespace sim
{
// ----------------------------------------------------------------------//

void CSquirrel::BindAll()
{
	Sqrat::Class<CEngine> CEngine(m_vm, "CEngine");

	CEngine.Func("GetTime", &CEngine::GetTime);
	CEngine.Func("GetDeltaTime", &CEngine::GetDeltaTime);

	m_rootTable->SetInstance("engine", CEngine::GetSingletonPtr());

	Sqrat::Class<CFileSystem> CFileSystem(m_vm, "CFileSystem");
	CFileSystem.Func("LinkEffect", &CFileSystem::LinkEffect);
}

// ----------------------------------------------------------------------//
}; // namespace sim
