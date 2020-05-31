#include <iostream>
#include "IPtrExt.h"

int main( int, char** )
{
    SPtr s;
    s = 2;
    std::cout << "s.i=" << s.get().i << std::endl;
    s = 3;
    std::cout << "s.i=" << s.get().i << std::endl;
    s = 4;
    std::cout << "s.i=" << s.get().i << std::endl;
}