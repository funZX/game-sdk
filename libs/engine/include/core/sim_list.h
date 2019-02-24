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

#ifndef __SIM_CLIST_H
#define __SIM_CLIST_H

#include <core/sim_pool.h>
#include <core/sim_core.h>

namespace sim
{
namespace stl
{
	// ------------------------------------------------------------------//
template <class T>
class CList
{
protected:
	// ------------------------------------------------------------------//
	template <class T>
	class CListNode
	{
	public:
		T				*m_data;
		CListNode<T>	*m_prev;
		CListNode<T>	*m_next;

		CListNode()
		{
			m_data		= nullptr;
			m_prev		= nullptr;
			m_next		= nullptr;
		}

		void Delete()
		{
			if (m_next) {
				m_next->m_prev = m_prev;
			}

			if (m_prev) {
				m_prev->m_next = m_next;
			}
		}
	};
	// ------------------------------------------------------------------//
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
	// ------------------------------------------------------------------//
	CListNode<T>* CList<T>::NewNode(T data, CListNode<T>* prev, CListNode<T>* next)
	{
		CListNode<T>* node = m_nodepool->New();

		node->m_prev = prev;
		node->m_next = next;
		node->m_data = m_datapool->New();

		SIM_MEMCPY(node->m_data, &data, sizeof(T));

		return node;
	}
	// ------------------------------------------------------------------//
	void CList<T>::DelNode(CListNode<T>* node)
	{
		node->Delete();

		m_datapool->Delete(node->m_data);
		m_nodepool->Delete(node);
	}

protected:													
	CListNode<T>*			m_first;
	CListNode<T>*			m_current;
	CListNode<T>*			m_last;
							
	s32 			 		m_count;
	CPool<CListNode<T>>*	m_nodepool;
	CPool<T>*				m_datapool;
};
// ------------------------------------------------------------------//

template <class T> CList<T>::CList()
{
	m_count		= 0;
	m_first 	= nullptr;
	m_last 		= nullptr;
	m_current 	= nullptr;

	m_nodepool = SIM_NEW CPool<CListNode<T>>();
	m_datapool = SIM_NEW CPool<T>();
}
// ------------------------------------------------------------------//

template <class T> CList<T>::~CList()
{
	SIM_SAFE_DELETE(m_datapool);
	SIM_SAFE_DELETE(m_nodepool);
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
	CListNode<T> *temp = NewNode( item, nullptr, m_first );

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
	CListNode<T> *temp = NewNode( item, m_last, nullptr );

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

	CListNode<T> *temp = NewNode( item, m_current, m_current->m_next );

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

	CListNode<T> *temp = NewNode( item, m_current->m_prev, m_current );

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
	DelNode( temp );

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

		DelNode(m_first);

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

	DelNode( temp );

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

} // namespace stl;
} // namespace sim;

#endif // __SIM_CLIST_H
