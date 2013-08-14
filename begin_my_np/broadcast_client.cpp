#include "iostream"
#include "sys/types.h"
#include "sys/socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "string.h"
#include "errno.h"
#include "ErrUtil.h"

/*
 *
 * =====================================================================================
 *
 *       Filename:  broadcast_client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/26/2013 04:23:44 AM
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
    sin.sin_addr.s_addr = inet_addr("192.168.1.255");
    sin.sin_port = htons(7777);
    // bOpt MUST be int, should not be bool!!
    int bOpt = 1;

    ErrUtil::printErrCode();
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &bOpt, sizeof(bOpt));
    ErrUtil::printErrCode();
    int ret = sendto(s, "hello broadcast", 15, 0, (sockaddr *) & sin,
                     sizeof(sockaddr));

    ErrUtil::printErrCode();
    return 0;
}
