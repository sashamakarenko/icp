#include <iostream>
#include "IPtrExt.h"

int main( int, char** )
{
    SPtr s;
    s = 2;
    std::cout << "s.i=" << s.get().i << std::endl;
    s = 3;
    std::cout << "s.i=" << s.get().i << std::endl;
    SPtr ss{10};
    std::cout << "ss.i=" << ss.get().i << std::endl;
    std::cout << "ss-s=" << ( &ss.get() - &s.get() ) << std::endl;
}
