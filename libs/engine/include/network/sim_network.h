#ifndef __SIM_NETWORK_H
#define __SIM_NETWORK_H

#include <core/sim_core.h>
#include <core/sim_singleton.h>

#ifdef LINUX
#	include <net/if.h>
#	include <fcntl.h>
#	include <netdb.h>
#	include <sys/select.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#endif


namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

class CNetwork : stl::CSingleton<CNetwork>
{
public:
	// ------------------------------------------------------------------//
	CNetwork();
	virtual ~CNetwork();
	// ------------------------------------------------------------------//

	void 					Init();
	void 					Term();

	inline std::wstring&	GetHostName() { return m_hostName; };
	inline s32				GetLocalIP() { return m_localIP; };

protected:
	void 					InitHostName();
	void 					InitLocalIP();

protected:
	// ------------------------------------------------------------------//
	std::wstring			m_hostName;
	s32						m_localIP;
	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim

#endif // __SIM_NETWORK_H
