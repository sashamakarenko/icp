#ifndef IPTREXT_H
#define IPTREXT_H

#include <icp/IPtr.h>
#include <iostream>
struct S
{
    S( int i ): i{i} { std::clog << "S::S( "<< i << " )\n"; }
    S(const S & other): i{ other.i } { std::clog << "S::S( S & "<< i << " )\n"; }
    ~S(){ std::clog << "~S("<< i << ")\n"; }
    int i;
};

typedef icp::IPtr<S> SPtr;


#endif /* IPTREXT_H */

