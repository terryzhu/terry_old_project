#include "SockClient.h"
#include "SockConn.h"
#include "ErrUtil.h"
SockClient::SockClient(const char *host, const char *service,
                       const char *protocal)
{
    this->s = SockConn::connectsock(host, service, protocal);
    if (s == -1)
    {
        ErrUtil::printErrCode();
    }
}

int
 SockClient::read(void *buf, int len)
{
    return::read(this->s, buf, len);
}

int SockClient::write(void *buf, int len)
{
    return::write(this->s, buf, len);
}
