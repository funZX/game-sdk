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

#ifndef __SIM_CQUEUE_H
#define __SIM_CQUEUE_H

#include <core/sim_core.h>
#include <core/sim_list.h>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//

template <class T> class CQueue
	: public CList<T>
{
public:
	CQueue() : CList<T>() {}
	virtual ~CQueue() {};

	virtual void Pop()
	{
		this->RemoveLast();
	}

	virtual void Push( T item )
	{
		this->AddToFront( item );
	}

	virtual T* Top()
	{
		return this->Last();
	}

	virtual u32 Length()
	{
		return this->Count();
	}
};

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_CQUEUE_H
