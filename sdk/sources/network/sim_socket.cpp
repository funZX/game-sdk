#include <network/sim_socket.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

CSocket :: CSocket()
{
	m_handle = CSocket::InvalidHandle;
}

CSocket :: ~CSocket()
{
	if( CSocket::InvalidHandle != m_handle )
		Close();
}

bool CSocket::Open( EProto proto )
{
	m_handle = socket( AF_INET, proto, 0 );

	return m_handle != CSocket::InvalidHandle;
}

bool CSocket::Close()
{
	result rst = close( m_handle );
	return rst != CSocket::ResultError;
}

bool CSocket::Bind( CEndpoint *end )
{
	sockaddr_in 			addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= end->GetIP();
	addr.sin_port			= htons( end->GetPort() );

	if( CSocket::ResultError == bind( m_handle, ( sockaddr* ) &addr, sizeof( addr ) ) )
	{
		SIM_ERR( "\nCSocket :: Bind failed!!!" );
		return false;
	}

	return true;
}

bool CSocket :: Connect( CEndpoint *end )
{
	sockaddr_in 			addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= end->GetIP();
	addr.sin_port			= htons( end->GetPort() );

	if( CSocket::ResultError == connect( m_handle, ( sockaddr* ) &addr, sizeof( addr ) ) )
	{
		SIM_ERR( "\nCSocket :: Connect failed!!!" );
		return false;
	}

	return true;
}

bool CSocket :: Accept( CEndpoint *end )
{
	sockaddr_in 			addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= end->GetIP();
	addr.sin_port			= htons( end->GetPort() );

	socklen_t addrlen = sizeof( addr );

	if( CSocket::ResultError == accept( m_handle, ( sockaddr* ) &addr, &addrlen ) )
	{
		SIM_ERR( "\nCSocket :: Accept failed!!!" );
		return false;
	}

	return true;
}

bool CSocket :: Listen( s32 que )
{
	if( CSocket::ResultError == listen( m_handle, que ) )
	{
		SIM_ERR( "\nCSocket :: Listen failed!!!" );
		return false;
	}

	return true;
}

result CSocket::Send( CEndpoint *end, const s8 *buf, s32 len )
{
	sockaddr_in addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= end->GetIP();
	addr.sin_port			= htons( end->GetPort() );

	socklen_t addrlen = sizeof( sockaddr_in );
	result rst = sendto( m_handle, buf, len, 0, ( const sockaddr* ) &addr, addrlen );

	return rst;
}

result CSocket::Receive( s8 *buf, s32 len, CEndpoint *end )
{
	SIM_ASS( buf != NULL );
	SIM_ASS( len > 0 );
	SIM_ASS( end != NULL );

	sockaddr_in addr;
	socklen_t addrlen = sizeof( sockaddr_in );

	s32 err = recvfrom( m_handle, buf, len, 0, ( sockaddr* )&addr, &addrlen );

	end->SetIP( ( s32 ) addr.sin_addr.s_addr );
	end->SetPort( ( u16 ) ntohs( addr.sin_port ) );

	return err;
}

result CSocket::SetSockOpt( s32 optname, const s8 *optval, s32 optlen )
{
	result rst = setsockopt( m_handle, SOL_SOCKET, optname, optval, optlen );
	return rst;
}


result CSocket::GetSockOpt( s32 optname, s8 *optval, s32 *optlen ) const
{
	result rst = getsockopt( m_handle, SOL_SOCKET, optname, optval, ( socklen_t* ) optlen );
	return rst;
}

void CSocket::SetNonBlocking()
{
	s32 arg;
	arg = fcntl( m_handle, F_GETFL, 0 );
	arg |= O_NONBLOCK;
	fcntl( m_handle, F_SETFL, arg );
}

bool CSocket::CanRead( u32 microsec )
{
	fd_set fds;
    struct timeval tv;

    FD_ZERO( &fds );
    FD_SET( m_handle, &fds );

    tv.tv_sec  = 0;
    tv.tv_usec = microsec;

    return ( ( select( m_handle + 1, &fds, 0, 0, &tv ) > 0 )
             && FD_ISSET( m_handle, &fds ) != 0 );
}

bool CSocket::CanWrite( u32 microsec )
{
	fd_set fds;
    struct timeval tv;

    FD_ZERO( &fds );
    FD_SET( m_handle, &fds );

    tv.tv_sec  = 0;
    tv.tv_usec = microsec;

    return ( ( select( m_handle + 1, 0, &fds, 0, &tv ) > 0 )
             && FD_ISSET( m_handle, &fds ) != 0 );
}
// ----------------------------------------------------------------------//
} // namespace std;
} // namespace sim;
