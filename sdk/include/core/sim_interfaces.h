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

#ifndef __SIM_INTERFACES_H
#define __SIM_INTERFACES_H

#include <core/sim_core.h>

namespace sim
{
// ----------------------------------------------------------------------//
namespace io { class CMemStream; };
namespace ren { class CDriver; class CCanvas; class CAnimationPose; class CBoneMask; };
namespace vm  { class CVirtualMachine; };

// ----------------------------------------------------------------------//

struct IScriptable
{
	virtual void 				BindToVirtualMachine( vm::CVirtualMachine *vm ) = 0;
};

// ----------------------------------------------------------------------//

struct IRenderable
{
	virtual void 				Render( ren::CDriver *driver ) = 0;
};

// ----------------------------------------------------------------------//

struct IUpdatable
{
	virtual void 				Update( f32 dt, void *userData ) = 0;
};

// ----------------------------------------------------------------------//

struct IAnimationBlend
{
	virtual void				Blend( ren::CAnimationPose *pose, ren::CBoneMask *mask ) = 0;
};

// ----------------------------------------------------------------------//

struct ISortable
{
public:
	virtual s32 				Compare( const ISortable *other ) = 0;
};

// ----------------------------------------------------------------------//

struct IDummySlot
{
	virtual void				OnDummySlot() = 0;
};

// ----------------------------------------------------------------------//

struct IState : public IUpdatable, public sigcxx::Trackable
{
public:
	typedef std::vector<std::string> TFsList;
	typedef std::function<void(void)> TFnDtor;

	IState(TFnDtor fn = std::function<void(void)>()): m_fnDtor(fn) {}
	virtual ~IState() { if (m_fnDtor) m_fnDtor(); }

    virtual void				ShowGui( ren::CCanvas* canvas ) = 0;
	virtual void				Render( ren::CDriver *driver ) = 0;

	virtual void				OnEnter()	= 0;
    virtual void				OnExit()	= 0;

protected:
	TFnDtor						m_fnDtor;
};

// ----------------------------------------------------------------------//

class IEngineItem
{
public:
	IEngineItem() { m_name = "none";  }
	IEngineItem( const std::string& name ) { m_name = name; }
	virtual ~IEngineItem() {}

	const std::string& 			GetName() { return m_name; }

    virtual bool				Load(io::CMemStream* ms) = 0;
    virtual bool				Save(io::CMemStream* ms) = 0;

protected:
	std::string					m_name;
};

// ----------------------------------------------------------------------//
}; // namespace sim

#endif // __SIM_INTERFACES_H
