#ifndef __SIM_BINARY_TREE_H
#define __SIM_BINARY_TREE_H

#include <core/sim_core.h>
#include <core/sim_memory_pool.h>
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

#define CTreeNode	CBinaryTreeNode<K, D>
#define CTree		CBinaryTree<K, D>

template <class K, class D> class CBinaryTreeNode
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

	virtual ~CBinaryTreeNode()
	{
	}

	void				SetKey( K key )				{ m_key = key; }
	K					GetKey()					{ return m_key; }

	void				SetData( D data )			{ m_data = data; }
	D					GetData()					{ return m_data; }

	void				SetParent( CTreeNode *node ){ m_parent = node; }
	CTreeNode*			GetParent()					{ return m_parent; }

	void				SetLeft( CTreeNode *node )	{ m_left = node; }
	CTreeNode*			GetLeft()					{ return m_left; }

	void				SetRight( CTreeNode *node )	{ m_right = node; }
	CTreeNode*			GetRight()					{ return m_right; }

	bool				IsLeft()					{ return m_parent->m_left == this; }
	bool				IsRight()					{ return m_parent->m_right == this; }
	bool				IsParent()					{ return m_left || m_right; }



// ----------------------------------------------------------------------//

	CTreeNode* GetSibling()
	{
		SIM_ASSERT ( GetParent() );

		return	IsLeft() ? 
					GetRight() : 
					GetLeft();
	}

// ----------------------------------------------------------------------//

	CTreeNode* GetUncle()
	{
		SIM_ASSERT ( GetParent() );
		SIM_ASSERT ( GetParent()->GetParent() );

		return Sibling( GetParent() );	
	}

// ----------------------------------------------------------------------//

	CTreeNode* GetGrandParent()
	{
		SIM_ASSERT ( GetParent() );
		SIM_ASSERT ( GetParent()->GetParent() );

		return GetParent()->GetParent();	
	}

//----------------------------------------------------------------------//
protected:
// ----------------------------------------------------------------------//
	K					m_key;
	D					m_data;

	CTreeNode *			m_parent;

	CTreeNode*			m_left;
	CTreeNode*			m_right;

// ----------------------------------------------------------------------//

	const CTreeNode& operator=( const CTreeNode& node )
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

template <class K, class D> 
class CBinaryTree
{
public:
// ----------------------------------------------------------------------//
	CBinaryTree()
	{
		m_root		= nullptr;
	}
	~CBinaryTree()
	{
		RemoveAll();
	}

private:
	// ----------------------------------------------------------------------//
	static CMemoryPool<CBinaryTreeNode<K, D>, k_Pool_Size * sizeof(CBinaryTreeNode<K, D>)>	 m_pool;
	// ----------------------------------------------------------------------//

protected:
	// ----------------------------------------------------------------------//	
	CTreeNode*				m_root;
	// ----------------------------------------------------------------------//	

public:
	// ----------------------------------------------------------------------//	
	inline CTreeNode*		GetRoot() { return m_root;  }
	// ----------------------------------------------------------------------//	

	void DeleteAll()
	{
		CStack<CTreeNode*> stack;
		stack.Push( m_root );

		while (stack.Count() > 0)
		{
			auto node = *stack.Top();
			stack.Pop();

			if (node == nullptr)
				continue;

			SIM_DELETE(node->GetData());

			stack.Push(node->GetLeft());
			stack.Push(node->GetRight());
		}

		RemoveAll();
	}

	// ----------------------------------------------------------------------//	
	void RemoveAll()
	{
		CTreeNode* subRoot	= m_root;
		CTreeNode* temp		= nullptr;
		
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

				DestroyNode( temp );
			}
		}

		m_root		= nullptr;
	}

// ----------------------------------------------------------------------//

	inline CTreeNode* Search( K key ) const
	{
		CTreeNode* subRoot = m_root;

		while( subRoot && key != subRoot->GetKey() )

			subRoot =	subRoot->GetKey() > key ? 
								subRoot->GetRight() : 
								subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CTreeNode* Min( CTreeNode* subRoot ) const
	{	
		while( subRoot && subRoot->GetLeft() )
			subRoot = subRoot->GetLeft();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CTreeNode* Max( CTreeNode* subRoot ) const
	{	
		while( subRoot && subRoot->GetRight() )
			subRoot = subRoot->GetRight();

		return subRoot;
	}

// ----------------------------------------------------------------------//

	CTreeNode* Successor( CTreeNode* subRoot ) const
	{
		if( subRoot->GetRight() )
			return Min( subRoot->GetRight() );

		CTreeNode* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetRight() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	CTreeNode* Predecessor( CTreeNode* subRoot ) const
	{	
		if( subRoot->GetLeft() )
			return Max( subRoot->GetLeft() );

		CTreeNode* subTree = subRoot->GetParent();

		while( subTree && subRoot == subTree->GetLeft() )
		{
			subRoot		= subTree;
			subTree		= subRoot->GetParent();
		}

		return subTree;
	}

// ----------------------------------------------------------------------//

	virtual CTreeNode* Insert( K key, D data )
	{
		SIM_ASSERT( !Search( key ) );

		CTreeNode* node		= nullptr;
		CTreeNode* subRoot	= m_root;

		while( subRoot )
		{
			node = subRoot;

			subRoot = subRoot->GetKey() > key ?
							subRoot->GetRight() :
							subRoot->GetLeft();
		}

		CTreeNode* newNode	= CreateNode();
		
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

		CTreeNode* nodeParent	= subTree->GetParent();

		if ( subTree->GetParent() )
		{
			bool isLeft = subTree->IsLeft();
			
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
		
		DestroyNode( subTree );

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

	void Print( CTreeNode* subRoot, u32 indent )
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

		IEngineItem* item = dynamic_cast<IEngineItem*>(subRoot->GetData());
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
	virtual CBinaryTreeNode<K, D>* CreateNode()
	{
		return m_pool.New();
	}

	virtual void DestroyNode(CBinaryTreeNode<K, D>* node)
	{
		m_pool.Delete( node );
	}
};

// ----------------------------------------------------------------------//
template <typename K, typename D>
CMemoryPool<CBinaryTreeNode<K,D>, k_Pool_Size * sizeof(CBinaryTreeNode<K, D>)> CBinaryTree<K,D>::m_pool;
// ----------------------------------------------------------------------//

#undef CTree
#undef CTreeNode

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim;

#endif // __SIM_BINARY_TREE_H