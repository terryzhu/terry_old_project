#include "SockConn.h"
#include "string.h"
#include "ErrUtil.h"
int
 SockConn::connectsock(const char *host, const char *service,
                       const char *transport)
{
    struct hostent *phe;        /*       host info */

    struct servent *pse;        /*   service info */

    struct protoent *ppe;       /*         protocal info */
    struct sockaddr_in sin = { 0 };
    int s, type;

    sin.sin_family = AF_INET;
    pse = getservbyname(service, transport);
    if (pse != NULL)
    {
        sin.sin_port = pse->s_port;
        std::cout << ntohs(sin.sin_port) << "\n";
    }

    phe = gethostbyname(host);
    if (phe != NULL)
    {
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    } else
    {
        sin.sin_addr.s_addr = inet_addr(host);
    }

    std::cout << inet_ntoa(sin.sin_addr);
    ppe = getprotobyname(transport);

    if (strcmp(transport, "tcp") == 0)
    {
        type = SOCK_STREAM;
    } else
    {
        type = SOCK_DGRAM;
    }

    s = socket(PF_INET, type, ppe->p_proto);

    int ret = connect(s, (struct sockaddr *) &sin, sizeof(sin));

    if (ret == -1)
    {
        ErrUtil::printErrCode();
    }
    return s;
}

int SockConn::connectTCP(const char *host, const char *service)
{
    return connectsock(host, service, "tcp");
}

int SockConn::connectUDP(const char *host, const char *service)
{
    return connectsock(host, service, "udp");
}
