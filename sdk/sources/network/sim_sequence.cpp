#include <network/sim_sequence.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

CSequence::CSequence()
{
	m_base		= 0;
	m_mask		= 0;
}

void CSequence::Recv( u32 seq )
{
	u32 offset = seq - m_base;

	if( offset < eMaskSize )
	{
		u32 seqBit = ( 1 << offset );
		m_mask |= seqBit;

		while( m_mask & 1 )
		{
			m_base++;
			m_mask >>= 1;
		}
	}
}


void CSequence::Send( u32 base, u32 mask )
{
	while( base > m_base )
	{
		m_base++;
		m_mask >>= 1;
	}
	m_mask |= mask;
}

bool CSequence::IsSent( u32 seq )
{
	const u32 offset = seq - m_base;
	return ( offset < 0 ) ||
		   ( ( offset >= 0 && offset < eMaskSize ) && ( m_mask & ( 1 << offset ) ) );
}

bool CSequence::IsRecv( u32 seq )
{
	const u32 offset = seq - m_base;
	return ( offset < 0 ) ||
		   ( ( offset >= 0 ) && ( m_mask & ( 1 << offset ) ) );
}

bool CSequence::IsNewer( CSequence *seq )
{
	return true;
}

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim
