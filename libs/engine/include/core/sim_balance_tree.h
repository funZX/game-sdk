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

#include <core/sim_pool.h>
#include <core/sim_core.h>
#include <core/sim_binary_tree.h>

#ifndef __SIM_BLACK_RED_TREE_H
#define __SIM_BLACK_RED_TREE_H

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
template <class K, class D> 
class CBalanceTree : public CBinaryTree<K, D>
{
protected:
	// ----------------------------------------------------------------------//
	template <class K, class D>
	class CBalanceTreeNode : public CBinaryTreeNode<K, D>
	{
	public:
		CBalanceTreeNode(K key, D* data) : 
			CBinaryTreeNode<K, D>(key, data)
		{
			m_isRed = false;
		}

		virtual ~CBalanceTreeNode() {}

#if SIM_DEBUG
		virtual const std::string N()
		{
			return m_isRed ? "(R)" : "(B)";
		}
#endif

	public:
		bool		m_isRed;
	};
	// ----------------------------------------------------------------------//
public:
	CBalanceTree() 
		: CBinaryTree<K, D>()
	{
		m_count			= 0;
		m_nodepool		= nullptr;
	}

	virtual ~CBalanceTree()
	{

	}
	inline u32 GetCount() { return m_count; }

private:
	// ----------------------------------------------------------------------//	
	CPool<CBalanceTreeNode<K, D>>*	m_nodepool;
	// ----------------------------------------------------------------------//	


protected:
	// ----------------------------------------------------------------------//
	u32								m_count;
	// ----------------------------------------------------------------------//

#if SIM_DEBUG
	void check_one( CBinaryTreeNode<K, D>* subRoot )
	{
		if ( subRoot == nullptr )
			return;

		if ( IsRedNode( subRoot ) )
		{
			SIM_ASSERT ( !subRoot->GetLeft()   || !IsRedNode( subRoot->GetLeft() ) );
			SIM_ASSERT ( !subRoot->GetRight()  || !IsRedNode( subRoot->GetRight() ) );
			SIM_ASSERT ( !subRoot->GetParent() || !IsRedNode( subRoot->GetParent() ) );
		}

		check_one( subRoot->GetLeft() );
		check_one( subRoot->GetRight() );
	}

// ----------------------------------------------------------------------//

	void check_two_helper( CBinaryTreeNode<K, D>* subRoot, s32 black_count, s32* path_black_count )
	{
		if ( !subRoot )
		{
			if (*path_black_count == -1)
			{
				*path_black_count = black_count;
			}
			else
			{
				SIM_ASSERT ( black_count == *path_black_count );
			}

			return;
		}

		if ( !IsRedNode( subRoot ) )
			black_count++;

		check_two_helper( subRoot->GetLeft(),  black_count, path_black_count );
		check_two_helper( subRoot->GetRight(), black_count, path_black_count );
	}

// ----------------------------------------------------------------------//

	void check_two( CBinaryTreeNode<K, D>* subRoot )
	{
		s32 black_count_path = -1;

		check_two_helper( m_root, 0, &black_count_path);
	}

// ----------------------------------------------------------------------//

	void check( CBinaryTreeNode<K, D>* subRoot )
	{
		check_one( subRoot );
		check_two( subRoot );
	}

#endif

	// ----------------------------------------------------------------------//

	void SetNodeColor(CBinaryTreeNode<K, D>* node, bool isRed)
	{
		CBalanceTreeNode<K, D>* bnode = static_cast<CBalanceTreeNode<K, D>*>(node);
		bnode->m_isRed = isRed;
	}
	// ----------------------------------------------------------------------//
	bool IsRedNode(CBinaryTreeNode<K, D>* node)
	{
		CBalanceTreeNode<K, D>* bnode = static_cast<CBalanceTreeNode<K, D>*>(node);
		return bnode && bnode->m_isRed;
	}
	// ----------------------------------------------------------------------//

// ----------------------------------------------------------------------//
public:
// ----------------------------------------------------------------------//

	void RotateLeft( CBinaryTreeNode<K, D>* subRoot )
	{
		CBinaryTreeNode<K, D>* subTree = subRoot->GetRight();

		subRoot->SetRight( subTree->GetLeft() );

		if ( subTree->GetLeft() )
			subTree->GetLeft()->SetParent( subRoot );

		subTree->SetParent( subRoot->GetParent() );

		if ( subRoot->GetParent() )
		{
			subRoot->IsLeft() ?
				subRoot->GetParent()->SetLeft( subTree ) :
				subRoot->GetParent()->SetRight( subTree );
		}
		else
			m_root = subTree;

		subTree->SetLeft( subRoot );
		subRoot->SetParent( subTree );
	}

	// ----------------------------------------------------------------------//

	void RotateRight( CBinaryTreeNode<K, D>* subRoot )
	{
		CBinaryTreeNode<K, D>* subTree = subRoot->GetLeft();

		subRoot->SetLeft( subTree->GetRight() );

		if ( subTree->GetRight() )
			subTree->GetRight()->SetParent( subRoot );

		subTree->SetParent( subRoot->GetParent() );

		if ( subRoot->GetParent() )
		{
			subRoot->IsRight() ?
				subRoot->GetParent()->SetRight( subTree ) :
				subRoot->GetParent()->SetLeft( subTree );
		}
		else
		{
			m_root = subTree;
		}

		subTree->SetRight( subRoot );
		subRoot->SetParent( subTree );
	}

	// ----------------------------------------------------------------------//

	virtual CBinaryTreeNode<K, D>* Insert( K key, D data )
	{
		SIM_ASSERT( !Search( key ) );

		CBinaryTreeNode<K, D>* newNode = CBinaryTree<K, D>::Insert( key, data );

		if ( newNode == m_root )
		{
			++m_count;
			return newNode;
		}

		CBinaryTreeNode<K, D>* subRoot = newNode;
		SetNodeColor( subRoot, true );

		while (IsRedNode( subRoot->GetParent() ) )
		{
		
			if ( subRoot->GetParent()->IsLeft() )
			{
				CBinaryTreeNode<K, D>* subTree = subRoot->GetGrandParent()->GetRight();

				if (IsRedNode( subTree ) )
				{
					SetNodeColor( subRoot->GetGrandParent(), true );

					SetNodeColor( subRoot->GetParent(), false );
					SetNodeColor( subTree, false );

					subRoot = subRoot->GetGrandParent();
				}
				else
				{
					if ( subRoot->IsRight() )
					{
						subRoot = subRoot->GetParent();

						RotateLeft( subRoot );

						SetNodeColor( subRoot->GetGrandParent(), true );
						SetNodeColor( subRoot->GetParent(), false );

						RotateRight( subRoot->GetGrandParent() );
					}
					else
					{
						SetNodeColor( subRoot->GetGrandParent(), true );
						SetNodeColor( subRoot->GetParent(), false );
						SetNodeColor( subRoot,  true );

						RotateRight( subRoot->GetGrandParent() );
					}
				}
			}
			else
			{
				CBinaryTreeNode<K, D>* subTree = subRoot->GetGrandParent()->GetLeft();

				if (IsRedNode( subTree ) )
				{
					SetNodeColor( subRoot->GetGrandParent(), true );

					SetNodeColor( subRoot->GetParent(), false );
					SetNodeColor( subTree, false );

					subRoot = subRoot->GetGrandParent();
				}
				else
				{
					if ( subRoot->IsLeft() )
					{
						subRoot = subRoot->GetParent();

						RotateRight( subRoot );

						SetNodeColor( subRoot->GetGrandParent(), true );
						SetNodeColor( subRoot->GetParent(), false );

						RotateLeft( subRoot->GetGrandParent() );
					}
					else
					{
						SetNodeColor( subRoot->GetGrandParent(), true );
						SetNodeColor( subRoot->GetParent(), false );
						SetNodeColor( subRoot,  true );

						RotateLeft( subRoot->GetGrandParent() );
					}
				}
			}
		}

		SetNodeColor( m_root, false );

#if SIM_DEBUG
		check( m_root );
#endif

		++m_count;

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

		bool isLeft			= false;
		CBinaryTreeNode<K, D>* nodeParent	= subTree->GetParent();

		if ( nodeParent )
		{
			isLeft = subTree->IsLeft();
			
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

		if ( !IsRedNode( subTree ) )
			DeleteFixUp( node, nodeParent, isLeft );

#if SIM_DEBUG
		check( m_root );
#endif

		DelNode( subTree );

		--m_count;

		return data;
	}

// ----------------------------------------------------------------------//

	D* Delete( K key )
	{	
		CBinaryTreeNode<K, D>* node	= CBalanceTree<K, D>::Search( key );

		SIM_ASSERT( node );

		return Delete( node );
	}

	// ----------------------------------------------------------------------//

	virtual void DeleteFixUp( CBinaryTreeNode<K, D>* subRoot, CBinaryTreeNode<K, D>* parent, bool isLeft )
	{
		while ( subRoot != m_root && !IsRedNode( subRoot ) )
		{
			CBinaryTreeNode<K, D>* subTree = nullptr;

			if ( isLeft ) 
			{
				subTree = parent->GetRight();

				if (IsRedNode( subTree ) )
				{
					SetNodeColor( subTree, false );
					SetNodeColor( parent, true );

					RotateLeft( parent );

					subTree = parent->GetRight();
				}

				if ( !IsRedNode( subTree->GetLeft() ) && !IsRedNode( subTree->GetRight() ) )
				{
					SetNodeColor( subTree, true );
					subRoot = parent;
					parent = subRoot->GetParent();
				} 
				else
				{
					if ( !IsRedNode( subTree->GetRight() ) )
					{
						SetNodeColor( subTree->GetLeft(), false );
						SetNodeColor( subTree, true );

						RotateRight( subTree );

						subTree = parent->GetRight();
					}

					SetNodeColor( subTree, IsRedNode( parent ) );
					SetNodeColor( parent, false );

					if ( subTree->GetRight() ) 
						SetNodeColor( subTree->GetRight(), false );

					RotateLeft( parent );
					subRoot		= m_root;
					parent		= nullptr;
				}
			} 
			else 
			{
				subTree = parent->GetLeft();
				
				if (IsRedNode( subTree ) )
				{
					SetNodeColor( subTree, false );
					SetNodeColor( parent, true );

					RotateRight( parent );

					subTree = parent->GetLeft();
				}

				if ( !IsRedNode( subTree->GetLeft() ) && !IsRedNode( subTree->GetRight() ) )
				{
					SetNodeColor( subTree, true );

					subRoot = parent;
					parent	= subRoot->GetParent();
				} 
				else
				{
					if ( !IsRedNode( subTree->GetLeft() ) )
					{
						SetNodeColor( subTree->GetRight(), false );
						SetNodeColor( subTree, true );

						RotateLeft( subTree );

						subTree = parent->GetLeft();
					}
					
					SetNodeColor( subTree, IsRedNode( parent ) );
					SetNodeColor( parent,false );

					if ( subTree->GetLeft() )
						SetNodeColor( subTree->GetLeft(), false );

					RotateRight( parent );

					subRoot = m_root;
					parent	= nullptr;
				}
			}
		}

		if ( subRoot )
			SetNodeColor( subRoot, false );
	}

	// ----------------------------------------------------------------------//

	virtual CBinaryTreeNode<K, D>* NewNode(K key, D data ) override
	{
		if (m_nodepool == nullptr)
		{
			m_nodepool = SIM_NEW CPool<CBalanceTreeNode<K, D>>();
			m_datapool = SIM_NEW CPool<D>();
		}
		
		return m_nodepool->New(key, m_datapool->New(data));
	}

	virtual void DelNode(CBinaryTreeNode<K, D>* node) override
	{
		m_datapool->Delete(node->m_data);
		m_nodepool->Delete(reinterpret_cast<CBalanceTreeNode<K, D>*>(node));
	}
// ----------------------------------------------------------------------//
};
// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BLACK_RED_TREE_H