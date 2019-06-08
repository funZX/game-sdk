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

#include <core/sim_octree.h>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
COctreeVolume::COctreeVolume()
{
    m_box       = axis::Origin.xyz;
    m_center    = axis::Origin.xyz;

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
COctree::COctree()
{
	m_root = nullptr;
	m_pool = SIM_NEW CPool<COctreeNode>();
}
// ----------------------------------------------------------------------//
COctree::~COctree()
{
	SIM_SAFE_DELETE(m_pool);
}
// ----------------------------------------------------------------------//
COctreeNode* COctree::NewNode()
{
	return m_pool->New();
}
// ----------------------------------------------------------------------//
void COctree::DelNode(COctreeNode* node)
{
	for (int k = 0; k < 8; k++)
		m_pool->Delete(node->m_octreenodes[k]);

	m_pool->Delete(node);
}

// ----------------------------------------------------------------------//
void COctree::Insert(COctreeVolume* volume)
{
	const u32 k_Split_Threshold = 16;

	if (!m_root)
	{
		m_root = NewNode();

		m_root->m_radius = zpl_vec3_max(volume->GetBox());
        m_root->m_center = volume->GetCenter();

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
	COctreeNode* newRoot = NewNode();

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
				DelNode(m_root->m_octreenodes[k]);

		m_root = m_root->m_octreenodes[full];
	}
	else
	{
		if (count == 0)
			DelNode(m_root);
	}
}
// ----------------------------------------------------------------------//
}; // namespace stl
}; // namespace sim;
