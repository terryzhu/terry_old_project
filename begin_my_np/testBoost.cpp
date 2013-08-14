
/*
* =====================================================================================
*
*       Filename:  testBoost.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  02/10/2013 10:37:47 PM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  YOUR NAME (), 
*   Organization:  
*
* =====================================================================================
*/

#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <algorithm>
#include "vector"
int main()
{
    using namespace boost::lambda;

    typedef std::istream_iterator < int >in;

    std::vector < int >v;

    std::string s;


    std::for_each(in(std::cin), in(), std::cout << (_1 * 3) << " ");

}
