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
#define CNode CBinaryTreeNode<KEY, DATA>
#define CTree CBalanceTree<KEY, DATA>

#define CNodeSetRed( x, c )				( ( CBalanceTreeNode<KEY, DATA>* ) (x) )->m_isRed = c
#define CNodeIsRed( x )					( (x) && ( ( CBalanceTreeNode<KEY, DATA>* ) (x) )->m_isRed )

template <class KEY, class DATA>
class CBalanceTreeNode : public CBinaryTreeNode<KEY, DATA>
{
public:
	CBalanceTreeNode() 
		: CBinaryTreeNode<KEY, DATA>()
	{
		m_isRed		= false;
	}

#if SIM_DEBUG
	virtual const std::string D()
	{
		return m_isRed ? "(R)" : "(B)";
	}
#endif

public:
	u32		m_isRed;
};

template <class KEY, class DATA> 
class CBalanceTree : public CBinaryTree<KEY, DATA>
{
public:
// ----------------------------------------------------------------------//
	CBalanceTree() 
		: CBinaryTree<KEY, DATA>()
	{
				m_count = 0;
	}

	~CBalanceTree()
	{

	}

	inline u32 GetCount() { return m_count; }
// ----------------------------------------------------------------------//
protected:
	u32			m_count;
// ----------------------------------------------------------------------//

#if SIM_DEBUG
	void check_one( CNode* subRoot )
	{
		if ( subRoot == NULL )
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

	void check_two_helper( CNode* subRoot, s32 black_count, s32* path_black_count )
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

	void check_two( CNode* subRoot )
	{
		s32 black_count_path = -1;

		check_two_helper( m_root, 0, &black_count_path);
	}

// ----------------------------------------------------------------------//

	void check( CNode* subRoot )
	{
		check_one( subRoot );
		check_two( subRoot );
	}

// ----------------------------------------------------------------------//

#endif

// ----------------------------------------------------------------------//
public:
// ----------------------------------------------------------------------//

	void RotateLeft( CNode* subRoot )
	{
		CNode* subTree = subRoot->GetRight();

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

	void RotateRight( CNode* subRoot )
	{
		CNode* subTree = subRoot->GetLeft();

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

	virtual CNode* Insert( KEY key, DATA data )
	{
		SIM_ASSERT( !Search( key ) );

		CNode* newNode		= CBinaryTree<KEY, DATA>::Insert( key, data );

		if ( newNode == m_root )
		{
			++m_count;
			return newNode;
		}

		CNode* subRoot		= newNode;
		CNodeSetRed( subRoot, true );

		while ( CNodeIsRed( subRoot->GetParent() ) )
		{
		
			if ( subRoot->GetParent()->IsLeft() )
			{
				CNode* subTree = subRoot->GetGrandParent()->GetRight();

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
				CNode* subTree = subRoot->GetGrandParent()->GetLeft();

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

	virtual DATA Delete( CNode* subRoot )
	{
		CNode *subTree = subRoot;

		if ( subRoot->GetLeft() && subRoot->GetRight() )
			subTree = Successor( subRoot );

		CNode *node = subTree->GetLeft() ?
							subTree->GetLeft() :
							subTree->GetRight();

		if ( node != NULL )
			node->SetParent( subTree->GetParent() );

		bool isLeft			= false;
		CNode* nodeParent	= subTree->GetParent();

		if ( nodeParent )
		{
			isLeft = subTree->IsLeft();
			
			isLeft ?
				subTree->GetParent()->SetLeft( node ) :
				subTree->GetParent()->SetRight( node );
		}
		else
			m_root = node;

		DATA data = subRoot->GetData();

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

		SIM_SAFE_DELETE( subTree );

		--m_count;

		return data;
	}

// ----------------------------------------------------------------------//

	DATA Delete( KEY key )
	{	
		CNode* node	= CTree::Search( key );

		SIM_ASSERT( node );

		return Delete( node );
	}

	// ----------------------------------------------------------------------//

	virtual void DeleteFixUp( CNode* subRoot, CNode* parent, bool isLeft )
	{
		while ( subRoot != m_root && !CNodeIsRed( subRoot ) )
		{
			CNode* subTree = NULL;

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
					parent		= NULL;
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
					parent	= NULL;
				}
			}
		}

		if ( subRoot )
			CNodeSetRed( subRoot, false );
	}

	// ----------------------------------------------------------------------//

	virtual CNode* CreateNode()
	{
		return SIM_NEW CBalanceTreeNode<KEY, DATA>();
	}

// ----------------------------------------------------------------------//
};

#undef CNodeSetRed
#undef CNodeIsRed

#undef CTree
#undef CNode
// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BLACK_RED_TREE_H