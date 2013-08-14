#include "iostream"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "vector"
#include "stdio.h"
#include "SockClient.h"
using namespace std;

int main()
{
    char buf[300] = { 0 };
    int n;
    SockClient client("localhost", "daytime", "tcp");
    while ((n = client.read(buf, 300)) > 0)
    {
        buf[n] = '\0';
        printf("%s", buf);
    }
    return 0;
}
