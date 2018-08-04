#include <core/sim_core.h>

#ifndef __SIM_BINARY_TREE_H
#define __SIM_BINARY_TREE_H

#if SIM_DEBUG
	#include <iostream>
	#include <iomanip>
#endif

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//

#define CNode CBinaryTreeNode<KEY, DATA>
#define CTree CBinaryTree<KEY, DATA>

template <class KEY, class DATA> class CBinaryTreeNode
{
public:
// ----------------------------------------------------------------------//
	CBinaryTreeNode()
	{
		m_parent		= nullptr;

		m_left			= nullptr;
		m_right			= nullptr;

		m_data			= nullptr;
		m_key			= 0;
	}

	~CBinaryTreeNode()
	{
	}

	void				SetKey( KEY key )			{ m_key = key; }
	KEY					GetKey()					{ return m_key; }

	void				SetData( DATA data )		{ m_data = data; }
	DATA				GetData()					{ return m_data; }

	void				SetParent( CNode *node )	{ m_parent = node; }
	CNode*				GetParent()					{ return m_parent; }

	void				SetLeft( CNode *node )		{ m_left = node; }
	CNode*				GetLeft()					{ return m_left; }

	void				SetRight( CNode *node )		{ m_right = node; }
	CNode*				GetRight()					{ return m_right; }

	bool				IsLeft()					{ return m_parent->m_left == this; }
	bool				IsRight()					{ return m_parent->m_right == this; }
	bool				IsParent()					{ return m_left || m_right; }

// ----------------------------------------------------------------------//

	CNode* GetSibling()
	{
		SIM_ASSERT ( GetParent() );

		return	IsLeft() ? 
					GetRight() : 
					GetLeft();
	}

// ----------------------------------------------------------------------//

	CNode* GetUncle()
	{
		SIM_ASSERT ( GetParent() );
		SIM_ASSERT ( GetParent()->GetParent() );

		return Sibling( GetParent() );	
	}

// ----------------------------------------------------------------------//

	CNode* GetGrandParent()
	{
		SIM_ASSERT ( GetParent() );
		SIM_ASSERT ( GetParent()->GetParent() );

		return GetParent()->GetParent();	
	}

#if SIM_DEBUG
	virtual const std::string D() 
	{
		return "";
	}

	template<typename DATA> 
	const std::string& GetName( DATA d );
#endif

// ----------------------------------------------------------------------//
protected:
// ----------------------------------------------------------------------//
	KEY								m_key;
	DATA							m_data;

	CNode *							m_parent;

	CNode*							m_left;
	CNode*							m_right;

// ----------------------------------------------------------------------//

	const CNode& operator=( const CNode& node )
	{
		m_parent		= node.m_parent;

		m_left			= node.m_left;
		m_right			= node.m_right;

		m_data			= node.m_data;
		m_key			= node.m_key;

		return *this;
	}

// ----------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//

template <class KEY, class DATA> 
class CBinaryTree
{
public:
// ----------------------------------------------------------------------//
	CBinaryTree()
	{
		m_root			= nullptr;
	}
	~CBinaryTree()
	{
		RemoveAll();
	}

// ----------------------------------------------------------------------//
public:
// ----------------------------------------------------------------------//	
	CNode*								m_root;
// ----------------------------------------------------------------------//	

	void RemoveAll()
	{
		CNode* subRoot	= m_root;
		CNode* temp		= nullptr;
		
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

				SIM_SAFE_DELETE( temp );
			}
		}

		m_root		= nullptr;
	}

// ----------------------------------------------------------------------//

	inline CNode* Search( KEY key ) const
	{
		CNode* subRoot = m_root;

		while( subRoot && key != subRoot->GetKey() )

			subRoot =	subRoot->GetKey() > key ? 
								subRoot->GetRight() : 
								subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CNode* Min( CNode* subRoot ) const
	{	
		while( subRoot && subRoot->GetLeft() )
			subRoot = subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CNode* Max( CNode* subRoot ) const
	{	
		while( subRoot && subRoot->GetRight() )
			subRoot = subRoot->GetRight();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CNode* Successor( CNode* subRoot ) const
	{
		if( subRoot->GetRight() )
			return Min( subRoot->GetRight() );

		CNode* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetRight() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	CNode* Predecessor( CNode* subRoot ) const
	{	
		if( subRoot->GetLeft() )
			return Max( subRoot->GetLeft() );

		CNode* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetLeft() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	virtual CNode* Insert( KEY key, DATA data )
	{
		SIM_ASSERT( !Search( key ) );

		CNode* node		= nullptr;
		CNode* subRoot	= m_root;

		while( subRoot )
		{
			node = subRoot;

			subRoot = subRoot->GetKey() > key ?
							subRoot->GetRight() :
							subRoot->GetLeft();
		}

		CNode* newNode	= CreateNode();
		
		newNode->SetKey( key );
		newNode->SetData( data );
		newNode->SetParent( node );

		if ( node )
		{
			node->GetKey() > key ?
				node->SetRight( newNode ):
				node->SetLeft( newNode );
		}
		else
			m_root = newNode;

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

		if ( node != nullptr )
			node->SetParent( subTree->GetParent() );

		CNode* nodeParent	= subTree->GetParent();

		if ( subTree->GetParent() )
		{
			bool isLeft = subTree->IsLeft();
			
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
		
		SIM_SAFE_DELETE( subTree );

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

#if SIM_DEBUG
	void Print( CNode* subRoot, u32 indent )
	{
		if( subRoot == nullptr )
			return;

		if( subRoot->GetRight() )
			Print( subRoot->GetRight(), indent + 6 );

		if ( indent )
			std::cout << std::setw(indent) << ' ';

		if ( subRoot->GetRight()) 
			std::cout<<" /\n" << std::setw(indent) << ' ';

		std::cout << GetName( subRoot->GetData() ).c_str() << subRoot->D() << "\n ";

		if( subRoot->GetLeft() )
		{
			std::cout << std::setw(indent) << ' ' <<" \\\n";
			Print( subRoot->GetLeft(), indent + 6 );
		}
	}
#endif
// ----------------------------------------------------------------------//

protected:
	virtual CNode* CreateNode()
	{
		return SIM_NEW CBinaryTreeNode<KEY, DATA>();
	}
};

#undef CTree
#undef CNode

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BINARY_TREE_H