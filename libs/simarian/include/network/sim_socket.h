#ifndef __SIM_SOCKET_H
#define __SIM_SOCKET_H

#include <network/sim_network.h>
#include <network/sim_endpoint.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

class CSocket
{
public:

	// ------------------------------------------------------------------//

	typedef enum
	{
		eProtoUdp = SOCK_DGRAM,
		eProtoTcp = SOCK_STREAM,
	} EProto;

	// ------------------------------------------------------------------//

	CSocket();
	virtual ~CSocket();

	// ------------------------------------------------------------------//
	bool				Open( EProto proto );
	bool				Close();

	bool				Bind( CEndpoint *end );
	bool				Connect( CEndpoint *end );
	bool				Accept( CEndpoint *end );
	bool				Listen( s32 que );

	result				Send( CEndpoint *end, const s8 *buf, s32 len );
	result				Receive( s8 *buf, s32 len, CEndpoint *end );

	result				SetSockOpt( s32 optname, const s8 *optval, s32 optlen );
	result				GetSockOpt( s32 optname, s8 *optval, s32 *optlen ) const;
	void				SetNonBlocking();

	bool				CanRead( u32 microsec = 0 );
	bool				CanWrite( u32 microsec = 0 );

	// ------------------------------------------------------------------//

	inline handle		GetHandle()		{ return m_handle; }

	// ------------------------------------------------------------------//

protected:
	// ------------------------------------------------------------------//

	handle				m_handle;

	// ------------------------------------------------------------------//

public:
	// ------------------------------------------------------------------//

	static const handle InvalidHandle 	= ( ( handle ) -1 );
	static const result ResultError 	= ( ( result ) -1 );

	// ------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
} // namespace std;
} // namespace sim;

#endif // __SIM_SOCKET_H
