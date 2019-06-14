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

#ifndef __SIM_BINARY_TREE_H
#define __SIM_BINARY_TREE_H

#include <core/sim_pool.h>
#include <core/sim_core.h>
#include <core/sim_stack.h>


#if SIM_DEBUG
	#include <iostream>
	#include <iomanip>
#endif

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
template <class K, class D> 
class CBinaryTree
{
protected:
	// ----------------------------------------------------------------------//
	template <class K, class D> class CBinaryTreeNode
	{
	public:
		// ----------------------------------------------------------------------//
		CBinaryTreeNode(K key, D* data)
		{
			m_parent	= nullptr;

			m_left		= nullptr;
			m_right		= nullptr;

			m_key		= key;
			m_data		= data;

			SIM_MEMCPY(m_data, data, sizeof(D));
		}

		virtual ~CBinaryTreeNode()
		{
		}

		void				SetParent(CBinaryTreeNode<K, D> *node) { m_parent = node; }
		CBinaryTreeNode<K, D>*			GetParent() { return m_parent; }

		void				SetLeft(CBinaryTreeNode<K, D> *node) { m_left = node; }
		CBinaryTreeNode<K, D>*			GetLeft() { return m_left; }

		void				SetRight(CBinaryTreeNode<K, D> *node) { m_right = node; }
		CBinaryTreeNode<K, D>*			GetRight() { return m_right; }

		bool				IsLeft() { return m_parent->m_left == this; }
		bool				IsRight() { return m_parent->m_right == this; }
		bool				IsParent() { return m_left || m_right; }



		// ----------------------------------------------------------------------//

		CBinaryTreeNode<K, D>* GetSibling()
		{
			SIM_ASSERT(GetParent());

			return	IsLeft() ?
				GetRight() :
				GetLeft();
		}

		// ----------------------------------------------------------------------//

		CBinaryTreeNode<K, D>* GetUncle()
		{
			SIM_ASSERT(GetParent());
			SIM_ASSERT(GetParent()->GetParent());

			return Sibling(GetParent());
		}

		// ----------------------------------------------------------------------//

		CBinaryTreeNode<K, D>* GetGrandParent()
		{
			SIM_ASSERT(GetParent());
			SIM_ASSERT(GetParent()->GetParent());

			return GetParent()->GetParent();
		}

		//----------------------------------------------------------------------//
		K								m_key;
		D*								m_data;
		// ----------------------------------------------------------------------//
	protected:
		CBinaryTreeNode<K, D> *			m_parent;

		CBinaryTreeNode<K, D>*			m_left;
		CBinaryTreeNode<K, D>*			m_right;

		// ----------------------------------------------------------------------//

		const CBinaryTreeNode<K, D>& operator=(const CBinaryTreeNode<K, D>& node)
		{
			m_parent = node.m_parent;

			m_left = node.m_left;
			m_right = node.m_right;

			m_data = node.m_data;
			m_key = node.m_key;

			return *this;
		}

		// ----------------------------------------------------------------------//
	};
	
	// ----------------------------------------------------------------------//
public:
	CBinaryTree()
	{
		m_root		= nullptr;
		m_nodepool	= nullptr;
		m_datapool	= nullptr;
	}

	virtual ~CBinaryTree()
	{
		SIM_SAFE_DELETE(m_datapool);
		SIM_SAFE_DELETE(m_nodepool);
	}

private:
	// ----------------------------------------------------------------------//	
	CPool<CBinaryTreeNode<K, D>>*	m_nodepool;
	// ----------------------------------------------------------------------//	
protected:
	// ----------------------------------------------------------------------//	
	CBinaryTreeNode<K, D>*			m_root;
	CPool<D>*						m_datapool;
	// ----------------------------------------------------------------------//	
	
public:
	// ----------------------------------------------------------------------//	
	inline CBinaryTreeNode<K, D>*		GetRoot() { return m_root;  }
	// ----------------------------------------------------------------------//	

    void Traverse(std::function<void(D)> callback)
    {
        CStack<CBinaryTreeNode<K, D>*> stack;
        stack.Push(m_root);

        while (stack.Count() > 0)
        {
            auto node = *stack.Top();
            stack.Pop();

            if (node == nullptr)
                continue;

            stack.Push(node->GetLeft());
            stack.Push(node->GetRight());
        }

        while (stack.Count() > 0)
        {
            auto node = *stack.Top();
            callback(*(node->m_data));
            stack.Pop();
        }
    }

    void DeleteAll()
    {
        Traverse([&, this](D data) {
            SIM_DELETE(data);
        });

        RemoveAll();
    }

	// ----------------------------------------------------------------------//	
	void RemoveAll()
	{
		CBinaryTreeNode<K, D>* subRoot	= m_root;
		CBinaryTreeNode<K, D>* temp		= nullptr;
		
		while( subRoot )
		{		
			if( subRoot->GetLeft() )
			{
				subRoot = subRoot->GetLeft();
			}
			else if( subRoot->GetRight() )
			{
				subRoot = subRoot->GetRight();
			}
			else											
			{	
				temp = subRoot;								

				if( subRoot->GetParent() )
				{
					subRoot->IsLeft() ?
						subRoot->GetParent()->SetLeft( nullptr ) :
						subRoot->GetParent()->SetRight( nullptr );
				}
				
				subRoot = subRoot->GetParent();

				DelNode( temp );
			}
		}

		m_root		= nullptr;
	}

// ----------------------------------------------------------------------//

	inline CBinaryTreeNode<K, D>* Search( K key ) const
	{
		CBinaryTreeNode<K, D>* subRoot = m_root;

		while( subRoot && key != subRoot->m_key )

			subRoot =	subRoot->m_key > key ? 
								subRoot->GetRight() : 
								subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CBinaryTreeNode<K, D>* Min( CBinaryTreeNode<K, D>* subRoot ) const
	{	
		while( subRoot && subRoot->GetLeft() )
			subRoot = subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CBinaryTreeNode<K, D>* Max( CBinaryTreeNode<K, D>* subRoot ) const
	{	
		while( subRoot && subRoot->GetRight() )
			subRoot = subRoot->GetRight();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CBinaryTreeNode<K, D>* Successor( CBinaryTreeNode<K, D>* subRoot ) const
	{
		if( subRoot->GetRight() )
			return Min( subRoot->GetRight() );

		CBinaryTreeNode<K, D>* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetRight() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	CBinaryTreeNode<K, D>* Predecessor( CBinaryTreeNode<K, D>* subRoot ) const
	{	
		if( subRoot->GetLeft() )
			return Max( subRoot->GetLeft() );

		CBinaryTreeNode<K, D>* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetLeft() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	virtual CBinaryTreeNode<K, D>* Insert( K key, D data )
	{
		SIM_ASSERT( !Search( key ) );

		CBinaryTreeNode<K, D>* node		= nullptr;
		CBinaryTreeNode<K, D>* subRoot	= m_root;

		while( subRoot )
		{
			node = subRoot;

			subRoot = subRoot->m_key > key ?
							subRoot->GetRight() :
							subRoot->GetLeft();
		}

		CBinaryTreeNode<K, D>* newNode	= NewNode(key, data);
		
		newNode->SetParent( node );

		if ( node )
		{
			node->m_key > key ?
				node->SetRight( newNode ):
				node->SetLeft( newNode );
		}
		else
			m_root = newNode;

		return newNode;
	}

// ----------------------------------------------------------------------//

	virtual D* Delete( CBinaryTreeNode<K, D>* subRoot )
	{
		CBinaryTreeNode<K, D> *subTree = subRoot;

		if ( subRoot->GetLeft() && subRoot->GetRight() )
			subTree = Successor( subRoot );

		CBinaryTreeNode<K, D> *node = subTree->GetLeft() ?
							subTree->GetLeft() :
							subTree->GetRight();

		if ( node != nullptr )
			node->SetParent( subTree->GetParent() );

		CBinaryTreeNode<K, D>* nodeParent	= subTree->GetParent();

		if ( subTree->GetParent() )
		{
			bool isLeft = subTree->IsLeft();
			
			isLeft ?
				subTree->GetParent()->SetLeft( node ) :
				subTree->GetParent()->SetRight( node );
		}
		else
			m_root = node;

		D* data = subRoot->m_data;

		if ( subRoot != subTree )
		{
			subRoot->m_key = subTree->m_key;
			subRoot->m_data = subTree->m_data;
		}
		
		DelNode( subTree );

		return data;
	}

// ----------------------------------------------------------------------//

	D* Delete( K key )
	{	
		CBinaryTreeNode<K, D>* node	= CBinaryTree<K, D>::Search( key );

		SIM_ASSERT( node );

		return Delete( node );
	}

// ----------------------------------------------------------------------//

	void Print( CBinaryTreeNode<K, D>* subRoot, u32 indent )
	{
#if SIM_DEBUG
		if( subRoot == nullptr )
			return;

		if( subRoot->GetRight() )
			Print( subRoot->GetRight(), indent + 6 );

		if ( indent )
			std::cout << std::setw(indent) << ' ';

		if ( subRoot->GetRight()) 
			std::cout<<" /\n" << std::setw(indent) << ' ';

		IEngineItem* item = dynamic_cast<IEngineItem*>(*(subRoot->m_data));
		if (item != nullptr)
			std::cout << item->GetName().c_str() << "\n ";

		if( subRoot->GetLeft() )
		{
			std::cout << std::setw(indent) << ' ' <<" \\\n";
			Print( subRoot->GetLeft(), indent + 6 );
		}
#endif

	}
// ----------------------------------------------------------------------//

protected:
	virtual CBinaryTreeNode<K, D>* NewNode( K key, D data )
	{
		if (m_nodepool == nullptr)
		{
			m_nodepool = SIM_NEW CPool<CBinaryTreeNode<K, D>>();
			m_datapool = SIM_NEW CPool<D>();
		}

		return m_nodepool->New(key, m_datapool->New(data));
	}

	virtual void DelNode(CBinaryTreeNode<K, D>* node)
	{
		m_datapool->Delete(node->m_data);
		m_nodepool->Delete(node);
	}
};

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BINARY_TREE_H