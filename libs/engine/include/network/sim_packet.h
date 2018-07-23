#ifndef __SIM_PACKET_H
#define __SIM_PACKET_H

#include <core/sim_memstream.h>
#include <network/sim_network.h>
#include <network/sim_endpoint.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

class CPacket : public io::CMemStream
{
public:
	CPacket();
	virtual ~CPacket();

	// ----------------------------------------------------------------------//
	typedef struct
{
		u8		m_reliable:1;
		u16		m_size:15;
		u32		m_sequence;
		u32		m_advSequence;
		u32		m_advBitfield;
	} THeader;

	enum
	{
		eHeaderSize	= sizeof( THeader ),
		eMaxSize	= 512,
	};


	// ----------------------------------------------------------------------//
	void				SendTo( CEndpoint *end, bool reliable );
	// ----------------------------------------------------------------------//

protected:
	// ----------------------------------------------------------------------//
	void				Pack();
	// ----------------------------------------------------------------------//

protected:
	THeader				m_header;
};

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim

#endif // __SIM_PACKET_H
