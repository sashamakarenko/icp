#ifndef PTREXT_H
#define PTREXT_H

#include <icp/Ptr.h>
#include <icp/IString.h>
#include <iostream>
#include <vector>
struct S
{
    S( int i ): i{i} { std::clog << "S::S( "<< i << " )\n"; }
    S(const S & other): i{ other.i } { std::clog << "S::S( S & "<< i << " )\n"; }
    ~S(){ std::clog << "~S("<< i << ")\n"; }
    int i;
};

typedef icp::Ptr<S> SPtr;

#endif /* PTREXT_H */
