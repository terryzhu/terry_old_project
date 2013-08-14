#include "iostream"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "vector"
#include "stdio.h"
class SockConn {
  public:
    static int connectsock(const char *host, const char *service,
                           const char *transport);
    static int connectTCP(const char *host, const char *service);
    static int connectUDP(const char *host, const char *service);
};
