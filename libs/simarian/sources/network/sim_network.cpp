#include <network/sim_network.h>

namespace sim
{
namespace net
{
// ----------------------------------------------------------------------//

CNetwork :: CNetwork()
{
	m_hostName = L"";
	m_localIP  = 0;
}

CNetwork :: ~CNetwork()
{

}

void CNetwork :: Init()
{
	InitHostName();
	InitLocalIP();
}

void CNetwork :: Term()
{

}

void CNetwork :: InitHostName()
{
	std::string hStr( "" );
	char hName[ 128 ];

	if( gethostname( hName, 128 ) == 0 ) {
		std::string hStr( hName );
		m_hostName.assign( hStr.begin(), hStr.end() );
	}
}

void CNetwork :: InitLocalIP()
{
	std::string hStr;
	hStr.assign( m_hostName.begin(), m_hostName.end() );

	struct hostent *phe = gethostbyname( hStr.c_str() );
	if( phe == NULL ) {
        SIM_PRT( "\nYow! Bad host lookup." );
        return;
    }

    for( s32 i = 0; phe->h_addr_list[i] != 0; ++i ) {

        struct in_addr addr;

        memcpy( &addr, phe->h_addr_list[i], sizeof( struct in_addr ) );

        if( addr.s_addr != INADDR_LOOPBACK ) {

            m_localIP = addr.s_addr;

            SIM_PRT( "CNetwork :: InitLocalIP: %s\n", inet_ntoa( addr ) );
            break;
        }
    }
}

// ----------------------------------------------------------------------//
}; // namespace net
}; // namespace sim
