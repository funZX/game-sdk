#ifndef __SIM_SEQUENCE_H
#define __SIM_SEQUENCE_H

#include <network/sim_network.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

class CSequence
{
	enum
	{
		eMaskSize = 8 * sizeof( u32 ),
	};

public:
	CSequence();

	void 					Recv( u32 seq );
	void 					Send( u32 base, u32 mask );

	bool 					IsSent( u32 seq );
	bool 					IsRecv( u32 seq );

	bool 					IsNewer( CSequence *seq );

	inline u32 				GetBase() { return m_base; }
	inline u32 				GetMask() { return m_mask; }

protected:
	u32						m_base;
	u32						m_mask;
};

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim

#endif // __SIM_SEQUENCE_H
