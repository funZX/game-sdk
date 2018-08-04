#include <core/sim_octree.h>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
COctreeVolume::COctreeVolume()
{
	Vec3ToZero(&m_box);
	Vec3ToZero(&m_center);

	m_radius = 1.0f;
}

// ----------------------------------------------------------------------//
void COctreeNode::OnOctreeUpdate(sigcxx::SLOT slot)
{

}
// ----------------------------------------------------------------------//
u32 COctreeNode::IsOutside( const COctreeVolume* volume )
{
	if (volume->GetMaxX() > GetMaxX())
		return 1;

	if (volume->GetMaxY() > GetMaxY())
		return 2;

	if (volume->GetMaxZ() > GetMaxZ())
		return 3;

	if (volume->GetMinX() < GetMinX())
		return -1;

	if (volume->GetMinY() < GetMinY())
		return -2;

	if (volume->GetMinZ() < GetMinZ())
		return -3;

	return 0;
}
// ----------------------------------------------------------------------//

void COctreeNode::Split()
{
}

// ----------------------------------------------------------------------//
void COctreeNode::NewVolume(COctreeVolume* volume)
{
	m_volumetree.Insert(volume->GetID(), volume);
	volume->OctreeUpdateSignal.Connect(this, &COctreeNode::OnOctreeUpdate);
}
// ----------------------------------------------------------------------//
void COctreeNode::DelVolume(COctreeVolume* volume)
{
	m_volumetree.Delete(volume->GetID());
	volume->OctreeUpdateSignal.Disconnect(this, &COctreeNode::OnOctreeUpdate);
}



// ----------------------------------------------------------------------//
CMemoryPool<COctreeNode, k_Pool_Size * sizeof(COctreeNode)> COctree::m_pool;
// ----------------------------------------------------------------------//
void COctree::Insert(COctreeVolume* volume)
{
	const u32 k_Split_Threshold = 16;

	if (!m_root)
	{
		m_root = CreateNode();

		m_root->m_radius = Vec3Max(volume->GetBox());
		Vec3Copy(&m_root->m_center, volume->GetCenter());

		m_root->NewVolume(volume);

		return;
	}

	while (u32 axis = m_root->IsOutside(volume))
		Expand(volume, axis);

	COctreeNode* subRoot = m_root;

	if (subRoot->m_volumetree.GetCount() > k_Split_Threshold)
		subRoot->Split();

	Shrink();
}
// ----------------------------------------------------------------------//
void COctree::Delete(COctreeVolume* volume)
{

}
// ----------------------------------------------------------------------//
void COctree::Expand(COctreeVolume* volume, u32 axis)
{
	COctreeNode* newRoot = CreateNode();

	newRoot->m_radius = m_root->m_radius * 2.0f;
	m_root = newRoot;
}
// ----------------------------------------------------------------------//
void COctree::Shrink()
{
	s32 count = 0, full = 0;

	for (u32 k = 0; k < 8; k++)
	{
		if (m_root->m_octreenodes[k])
		{
			full = k;
			++count;
		}
	}

	if (count == 1)
	{
		for (u32 k = 0; k < 8; k++)
			if (k != full)
				DestroyNode(m_root->m_octreenodes[k]);

		m_root = m_root->m_octreenodes[full];
	}
	else
	{
		if (count == 0)
			DestroyNode(m_root);
	}
}
// ----------------------------------------------------------------------//
}; // namespace stl
}; // namespace sim;
