#include <iostream>
#include "PtrExt.h"

int main( int, char** )
{
    SPtr s;
    s = 2;
    std::cout << "s.i=" << s->i << std::endl;
    s = 3;
    std::cout << "s.i=" << s->i << std::endl;
    SPtr ss{10};
    std::cout << "ss.i=" << ss->i << std::endl;
    std::cout << "ss-s=" << ( ss.get() - s.get() ) << std::endl;
}
