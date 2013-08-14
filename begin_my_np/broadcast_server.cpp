#include "iostream"
#include "sys/types.h"
#include "sys/socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "string.h"

/*
 * =====================================================================================
 *
 *       Filename:  broadcast_server.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/26/2013 04:08:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
int main()
{
    int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in sin;

    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(7777);
    bind(s, (struct sockaddr *) &sin, sizeof(sin));
    char buf[100] = { 0 };
    struct sockaddr_in senderAddr;

    socklen_t len = sizeof(struct sockaddr_in);

    recvfrom(s, buf, 100, 0, (sockaddr *) & senderAddr, &len);

    std::cout << buf << "\n";
    return 0;
}
