#ifndef IPTREXT_H
#define IPTREXT_H

#include <icp/IPtr.h>

struct S
{
    S( int i ): i{i} {}
    int i;
};

typedef icp::IPtr<S> SPtr;


#endif /* IPTREXT_H */

