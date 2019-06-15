#include <network/sim_packet.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

CPacket :: CPacket()
		: sim::io::CMemStream( CPacket::eMaxSize )
{

}

CPacket :: ~CPacket()
{

}

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim
