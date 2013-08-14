#include "ErrUtil.h"
#include "errno.h"
#include "stdio.h"
#include "string.h"

/*
 * =====================================================================================
 *
 *       Filename:  ErrUtil.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/26/2013 05:21:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

void
 ErrUtil::printErrCode()
{
    printf("Err code:[%d] Info:[%s]\n", errno, strerror(errno));
}
