#ifndef __SIM_ENDPOINT_H
#define __SIM_ENDPOINT_H

#include <network/sim_network.h>
#include <network/sim_sequence.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

class CEndpoint
{
public:
	CEndpoint();
	virtual ~CEndpoint();

	// ----------------------------------------------------------------------//

	typedef enum
	{
		eAddrAnyone		= INADDR_ANY,
		eAddrBroadcast  = INADDR_BROADCAST,
		eAddrLoopback	= INADDR_LOOPBACK,
		eAddrNone		= INADDR_NONE,
	} EAddr;

	// ----------------------------------------------------------------------//
	inline s32				GetIP()   const { return m_ip;   }
	inline u16				GetPort() const { return m_port; }

	inline void				SetIP( s32 ip ) { m_ip = ip;   }
	inline void				SetPort( u16 port ) { m_port = port; }

	// ----------------------------------------------------------------------//

protected:
	s32						m_ip;
	u16						m_port;

	CSequence				m_localSeq;
	CSequence				m_remotSeq;
};

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim

#endif // __SIM_ENDPOINT_H
