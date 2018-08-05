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

#ifndef __SIM_CLIST_H
#define __SIM_CLIST_H

#include <core/sim_core.h>
#include <core/sim_memory_pool.h>

namespace sim
{
namespace stl
{
// ------------------------------------------------------------------//
template <class T> class CList;
// ------------------------------------------------------------------//
template <class T>
class CListNode
{
	friend class CList<T>;

protected:
	T												*m_data;
	CListNode<T>									*m_prev;
	CListNode<T>									*m_next;
	static CMemoryPool<T, k_Pool_Size * sizeof(T)>	 m_pool;

public:
	CListNode(T data, CListNode<T> *pPrev = nullptr, CListNode<T> *pNext = nullptr)
	{
		m_data = m_pool.New();
		SIM_MEMCPY(m_data, &data, sizeof(T));

		m_prev = pPrev;
		m_next = pNext;
	}

	~CListNode()
	{
		if (m_next) {
			m_next->m_prev = m_prev;
		}

		if (m_prev) {
			m_prev->m_next = m_next;
		}

		m_pool.Delete(m_data);
	}
};

// ------------------------------------------------------------------//
template <typename T> CMemoryPool<T, k_Pool_Size * sizeof(T)> CListNode<T>::m_pool;
// ------------------------------------------------------------------//

template <class T>
class CList
{
public:
	CList();
	virtual ~CList();
	
	bool													IsEmpty( void );
	u32														Count( void );
															
	void													AddToFront( T item );
	void													AddToEnd( T item );
	void													InsertAfter( T item );
	void													InsertBefore( T item );
															
	void													Begin();
	T*														First();
	T*														Next();
	T*														Current();
	T*														Last();
															
	void 													RemoveFirst();
	void 													Remove( T *item );
	void 													RemoveAll();
	void 													RemoveLast();
															
	void 													MoveToEnd( T *item );
	void 													MoveToFront( T *item );
															
protected:													
	CListNode<T>*												m_first;
	CListNode<T>*												m_current;
	CListNode<T>*												m_last;
															 
	s32 			 											m_count;
	static CMemoryPool<CListNode<T>, k_Pool_Size * sizeof(T)>	m_pool;
};
// ------------------------------------------------------------------//

template <class T> CList<T>::CList()
{
	m_count		= 0;
	m_first 	= nullptr;
	m_last 		= nullptr;
	m_current 	= nullptr;
}
// ------------------------------------------------------------------//

template <class T> CList<T>::~CList()
{
	m_current = m_first;

	while( m_current && m_first )
	{
		m_first = m_first->m_next;

		m_pool.Delete( m_current );

		m_current = m_first;
	}
}
// ------------------------------------------------------------------//
template <class T> bool CList<T>::IsEmpty()
{
	return ( m_first == nullptr );
}
// ------------------------------------------------------------------//
template<class T> u32 CList<T>::Count()
{
	return m_count;
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::AddToFront(T item )
{
	CListNode<T> *temp = m_pool.New( item, nullptr, m_first );

	if( m_first )
	{
		m_first->m_prev = temp;
		m_first = temp;
	}
	else
	{
		m_first = temp;

		m_current = m_first;
		m_last = m_first;
	}

	m_count++;
}
// ------------------------------------------------------------------//
template <class T> void CList<T>::AddToEnd( T item )
{
	CListNode<T> *temp = m_pool.New( item, m_last, nullptr );

	if( m_last)
	{

		m_last->m_next = temp;
		m_last = temp;
	}
	else
	{
		m_last = temp;

		m_first = m_last;
		m_current = m_last;
	}

	m_count++;
}
// ------------------------------------------------------------------//
template <class T> void CList<T>::InsertAfter( T item )
{
	SIM_ASSERT( m_current );

	CListNode<T> *temp = m_pool.New( item, m_current, m_current->m_next );

	if( m_current->m_next ) {
		m_current->m_next->prev = temp;
	}

	m_current->m_next = temp;

	m_count++;
}

// ------------------------------------------------------------------//

template <class T> void CList<T>::InsertBefore( T item )
{
	SIM_ASSERT( m_current );

	CListNode<T> *temp = m_pool.New( item, m_current->m_prev, m_current );

	if( m_current->m_prev ) {
		m_current->m_prev->m_next = temp;
	}

	m_current->prev = temp;

	m_count++;
}
// ------------------------------------------------------------------//
template <class T> void CList<T>::Begin()
{
	m_current = m_first;
}

template <class T> T* CList<T>::First()
{
	if( m_first == nullptr)
		return nullptr;

	return m_first->m_data;
}
// ------------------------------------------------------------------//
template <class T> T* CList<T>::Next()
{
	if( !m_current )
		return nullptr;

	T* temp 	= m_current->m_data;
	m_current 	= m_current->m_next;

	return temp;
}
// ------------------------------------------------------------------//
template <class T> T* CList<T>::Last()
{
	return m_last == nullptr ? nullptr : m_last->m_data;
}
// ------------------------------------------------------------------//
template <class T> T* CList<T>::Current()
{
	if( m_current == nullptr)
		return nullptr;

	return m_current->m_data;
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::RemoveFirst()
{
	if( m_first == nullptr ) {
		return;
	}

	if( m_first->m_next ) {
		m_first->m_next->m_prev = nullptr;
	}

	if( m_first == m_last ) {
		m_last = nullptr;
	}

	CListNode<T> *temp = m_first;
	m_first = m_first->m_next;
	m_pool.Delete( temp );

	m_count--;
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::Remove( T *item )
{
	CListNode<T> *temp = m_first;

	while( temp )
	{
		if( temp->m_data == item )
		{
			if( temp->m_next )
				temp->m_next->m_prev = temp->m_prev;
		
			if( temp->m_prev )
				temp->m_prev->m_next = temp->m_next;

			if ( temp == m_first )
				m_first = nullptr;

			SIM_SAFE_DELETE( temp );

			m_count--;

			return;
		}

		temp = temp->m_next;
	}
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::RemoveAll()
{
	CListNode<T> *temp = nullptr;

	while( m_first )
	{
		temp	= m_first->m_next;

		SIM_SAFE_DELETE(m_first);

		m_first	= temp;
	}

	m_first = m_last = m_current = nullptr;
	m_count	= 0;
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::RemoveLast()
{
	if( m_last == nullptr ) {
		return;
	}

	if( m_last->m_prev) {
		m_last->m_prev->m_next = nullptr;
	}

	if( m_first == m_last ) {
		m_first = nullptr;
	}

	CListNode<T> *temp = m_last;

	m_last = m_last->m_prev;

	SIM_SAFE_DELETE temp;

	m_count--;
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::MoveToFront( T *item )
{
	CListNode<T> *temp;

	if( m_first->m_data	== item )
		return;

	temp = m_first;
	
	while( temp != nullptr )
	{
		if( temp->m_data == item )
		{
			if( m_last == temp ) {
				m_last	= m_last->m_prev;
			}

			if( temp->m_next ) {
				temp->m_next->m_prev = temp->m_prev;
			}

			if( temp->m_prev ) {
				temp->m_prev->m_next = temp->m_next;
			}

			temp->m_prev	= nullptr;
			temp->m_next	= m_first;

			if( m_first ) {
				m_first->m_prev	= temp;
			}

			m_first			= temp;

			return;
		}

		temp = temp->m_next;
	}
}
// ------------------------------------------------------------------//
template<class T> void CList<T>::MoveToEnd( T *item )
{
	CListNode<T> *temp;

	if( m_last->m_data == item )
		return;

	temp = m_first;
	
	while( temp != nullptr )
	{
		if( temp->m_data == item )
		{
			if( m_first == temp ) {
				m_first	= m_first->m_next;
			}

			if( temp->m_next ) {
				temp->m_next->m_prev = temp->m_prev;
			}
			
			if( temp->m_prev ) {
				temp->m_prev->m_next = temp->m_next;
			}

			temp->m_prev = m_last;
			temp->m_next = nullptr;

			if(m_last) {
				m_last->next = temp;
			}

			m_last = temp;

			return;
		}

		temp = temp->m_next;
	}
}

// ----------------------------------------------------------------------//
template <typename T> CMemoryPool<CListNode<T>, k_Pool_Size * sizeof(T)> CList<T>::m_pool;
// ------------------------------------------------------------------//

} // namespace stl;
} // namespace sim;

#endif // __SIM_CLIST_H
