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

#include <core/sim_core.h>
#include <core/sim_binary_tree.h>

#ifndef __SIM_BLACK_RED_TREE_H
#define __SIM_BLACK_RED_TREE_H

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
#define CTreeNode	CBinaryTreeNode<K, D>
#define CTree		CBalanceTree<K, D>

#define CNodeSetRed( x, c )				( ( CBalanceTreeNode<K, D>* ) (x) )->m_isRed = c
#define CNodeIsRed( x )					( (x) && ( ( CBalanceTreeNode<K, D>* ) (x) )->m_isRed )

template <class K, class D>
class CBalanceTreeNode : public CBinaryTreeNode<K, D>
{
public:
	CBalanceTreeNode() : CBinaryTreeNode<K, D>()
	{
		m_isRed		= false;
	}

	virtual ~CBalanceTreeNode() {}

#if SIM_DEBUG
	virtual const std::string N()
	{
		return m_isRed ? "(R)" : "(B)";
	}
#endif

public:
	u32		m_isRed;
};

template <class K, class D> 
class CBalanceTree : public CBinaryTree<K, D>
{
public:
// ----------------------------------------------------------------------//
	CBalanceTree() 
		: CBinaryTree<K, D>()
	{
				m_count = 0;
	}

	~CBalanceTree()
	{

	}

	inline u32 GetCount() { return m_count; }
private:
	// ----------------------------------------------------------------------//
	static CMemoryPool<CBalanceTreeNode<K, D>, k_Pool_Size * sizeof(CBalanceTreeNode<K, D>)>	 m_pool;
	// ----------------------------------------------------------------------//
// ----------------------------------------------------------------------//
protected:
	u32			m_count;
// ----------------------------------------------------------------------//

#if SIM_DEBUG
	void check_one( CTreeNode* subRoot )
	{
		if ( subRoot == nullptr )
			return;

		if ( CNodeIsRed( subRoot ) )
		{
			SIM_ASSERT ( !subRoot->GetLeft()   || !CNodeIsRed( subRoot->GetLeft() ) );
			SIM_ASSERT ( !subRoot->GetRight()  || !CNodeIsRed( subRoot->GetRight() ) );
			SIM_ASSERT ( !subRoot->GetParent() || !CNodeIsRed( subRoot->GetParent() ) );
		}

		check_one( subRoot->GetLeft() );
		check_one( subRoot->GetRight() );
	}

// ----------------------------------------------------------------------//

	void check_two_helper( CTreeNode* subRoot, s32 black_count, s32* path_black_count )
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

		if ( !CNodeIsRed( subRoot ) )
			black_count++;

		check_two_helper( subRoot->GetLeft(),  black_count, path_black_count );
		check_two_helper( subRoot->GetRight(), black_count, path_black_count );
	}

// ----------------------------------------------------------------------//

	void check_two( CTreeNode* subRoot )
	{
		s32 black_count_path = -1;

		check_two_helper( m_root, 0, &black_count_path);
	}

// ----------------------------------------------------------------------//

	void check( CTreeNode* subRoot )
	{
		check_one( subRoot );
		check_two( subRoot );
	}

// ----------------------------------------------------------------------//

#endif

// ----------------------------------------------------------------------//
public:
// ----------------------------------------------------------------------//

	void RotateLeft( CTreeNode* subRoot )
	{
		CTreeNode* subTree = subRoot->GetRight();

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

	void RotateRight( CTreeNode* subRoot )
	{
		CTreeNode* subTree = subRoot->GetLeft();

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

	virtual CTreeNode* Insert( K key, D data )
	{
		SIM_ASSERT( !Search( key ) );

		CTreeNode* newNode		= CBinaryTree<K, D>::Insert( key, data );

		if ( newNode == m_root )
		{
			++m_count;
			return newNode;
		}

		CTreeNode* subRoot		= newNode;
		CNodeSetRed( subRoot, true );

		while ( CNodeIsRed( subRoot->GetParent() ) )
		{
		
			if ( subRoot->GetParent()->IsLeft() )
			{
				CTreeNode* subTree = subRoot->GetGrandParent()->GetRight();

				if ( CNodeIsRed( subTree ) )
				{
					CNodeSetRed( subRoot->GetGrandParent(), true );

					CNodeSetRed( subRoot->GetParent(), false );
					CNodeSetRed( subTree, false );

					subRoot = subRoot->GetGrandParent();
				}
				else
				{
					if ( subRoot->IsRight() )
					{
						subRoot = subRoot->GetParent();

						RotateLeft( subRoot );

						CNodeSetRed( subRoot->GetGrandParent(), true );
						CNodeSetRed( subRoot->GetParent(), false );

						RotateRight( subRoot->GetGrandParent() );
					}
					else
					{
						CNodeSetRed( subRoot->GetGrandParent(), true );
						CNodeSetRed( subRoot->GetParent(), false );
						CNodeSetRed( subRoot,  true );

						RotateRight( subRoot->GetGrandParent() );
					}
				}
			}
			else
			{
				CTreeNode* subTree = subRoot->GetGrandParent()->GetLeft();

				if ( CNodeIsRed( subTree ) )
				{
					CNodeSetRed( subRoot->GetGrandParent(), true );

					CNodeSetRed( subRoot->GetParent(), false );
					CNodeSetRed( subTree, false );

					subRoot = subRoot->GetGrandParent();
				}
				else
				{
					if ( subRoot->IsLeft() )
					{
						subRoot = subRoot->GetParent();

						RotateRight( subRoot );

						CNodeSetRed( subRoot->GetGrandParent(), true );
						CNodeSetRed( subRoot->GetParent(), false );

						RotateLeft( subRoot->GetGrandParent() );
					}
					else
					{
						CNodeSetRed( subRoot->GetGrandParent(), true );
						CNodeSetRed( subRoot->GetParent(), false );
						CNodeSetRed( subRoot,  true );

						RotateLeft( subRoot->GetGrandParent() );
					}
				}
			}
		}

		CNodeSetRed( m_root, false );

#if SIM_DEBUG
		check( m_root );
#endif

		++m_count;

		return newNode;
	}

	// ----------------------------------------------------------------------//

	virtual D Delete( CTreeNode* subRoot )
	{
		CTreeNode *subTree = subRoot;

		if ( subRoot->GetLeft() && subRoot->GetRight() )
			subTree = Successor( subRoot );

		CTreeNode *node = subTree->GetLeft() ?
							subTree->GetLeft() :
							subTree->GetRight();

		if ( node != nullptr )
			node->SetParent( subTree->GetParent() );

		bool isLeft			= false;
		CTreeNode* nodeParent	= subTree->GetParent();

		if ( nodeParent )
		{
			isLeft = subTree->IsLeft();
			
			isLeft ?
				subTree->GetParent()->SetLeft( node ) :
				subTree->GetParent()->SetRight( node );
		}
		else
			m_root = node;

		D data = subRoot->GetData();

		if ( subRoot != subTree )
		{
			subRoot->SetKey( subTree->GetKey() );
			subRoot->SetData( subTree->GetData() );
		}

		if ( !CNodeIsRed( subTree ) )
			DeleteFixUp( node, nodeParent, isLeft );

#if SIM_DEBUG
		check( m_root );
#endif

		DestroyNode( subTree );

		--m_count;

		return data;
	}

// ----------------------------------------------------------------------//

	D Delete( K key )
	{	
		CTreeNode* node	= CTree::Search( key );

		SIM_ASSERT( node );

		return Delete( node );
	}

	// ----------------------------------------------------------------------//

	virtual void DeleteFixUp( CTreeNode* subRoot, CTreeNode* parent, bool isLeft )
	{
		while ( subRoot != m_root && !CNodeIsRed( subRoot ) )
		{
			CTreeNode* subTree = nullptr;

			if ( isLeft ) 
			{
				subTree = parent->GetRight();

				if ( CNodeIsRed( subTree ) )
				{
					CNodeSetRed( subTree, false );
					CNodeSetRed( parent, true );

					RotateLeft( parent );

					subTree = parent->GetRight();
				}

				if ( !CNodeIsRed( subTree->GetLeft() ) && !CNodeIsRed( subTree->GetRight() ) )
				{
					CNodeSetRed( subTree, true );
					subRoot = parent;
					parent = subRoot->GetParent();
				} 
				else
				{
					if ( !CNodeIsRed( subTree->GetRight() ) )
					{
						CNodeSetRed( subTree->GetLeft(), false );
						CNodeSetRed( subTree, true );

						RotateRight( subTree );

						subTree = parent->GetRight();
					}

					CNodeSetRed( subTree, CNodeIsRed( parent ) );
					CNodeSetRed( parent, false );

					if ( subTree->GetRight() ) 
						CNodeSetRed( subTree->GetRight(), false );

					RotateLeft( parent );
					subRoot		= m_root;
					parent		= nullptr;
				}
			} 
			else 
			{
				subTree = parent->GetLeft();
				
				if ( CNodeIsRed( subTree ) )
				{
					CNodeSetRed( subTree, false );
					CNodeSetRed( parent, true );

					RotateRight( parent );

					subTree = parent->GetLeft();
				}

				if ( !CNodeIsRed( subTree->GetLeft() ) && !CNodeIsRed( subTree->GetRight() ) )
				{
					CNodeSetRed( subTree, true );

					subRoot = parent;
					parent	= subRoot->GetParent();
				} 
				else
				{
					if ( !CNodeIsRed( subTree->GetLeft() ) )
					{
						CNodeSetRed( subTree->GetRight(), false );
						CNodeSetRed( subTree, true );

						RotateLeft( subTree );

						subTree = parent->GetLeft();
					}
					
					CNodeSetRed( subTree, CNodeIsRed( parent ) );
					CNodeSetRed( parent,false );

					if ( subTree->GetLeft() )
						CNodeSetRed( subTree->GetLeft(), false );

					RotateRight( parent );

					subRoot = m_root;
					parent	= nullptr;
				}
			}
		}

		if ( subRoot )
			CNodeSetRed( subRoot, false );
	}

	// ----------------------------------------------------------------------//

	virtual CBinaryTreeNode<K, D>* CreateNode()
	{
		return m_pool.New();
	}

	virtual void DestroyNode(CBinaryTreeNode<K, D>* node)
	{
		m_pool.Delete(reinterpret_cast<CBalanceTreeNode<K, D>*>(node));
	}
// ----------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
template <typename K, typename D>
CMemoryPool<CBalanceTreeNode<K, D>, k_Pool_Size * sizeof(CBalanceTreeNode<K, D>)> CBalanceTree<K, D>::m_pool;
// ----------------------------------------------------------------------//

#undef CNodeSetRed
#undef CNodeIsRed

#undef CTree
#undef CTreeNode
// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BLACK_RED_TREE_H