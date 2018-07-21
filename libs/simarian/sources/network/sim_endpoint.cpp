#include <network/sim_endpoint.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

CEndpoint :: CEndpoint()
{
	m_port 		= 0;
	m_ip	 	= 0;
}

CEndpoint :: ~CEndpoint()
{

}

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim
