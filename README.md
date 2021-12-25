# icp - Immutable and Compact Pointers
Using 64-bit pointers and modern std::string makes your objects quite large and may increase cache miss probability.

Suppose you run a webstore and maintain an in-memory database:
```
                                   +--------------------+
   +-----------------+  1          |       Customer     |
   |      Order      o------------>|--------------------+
   |-----------------|             | firstName: string  |
   | id: string      |<------------o secondName: string |
   | state: State    |          *  | email: string      |
   +--------o--------+             +--------------------+
            | *
            |
            |
            v                     +-----------------------+
 +--------------------+           |        Product        |
 |        Item        |    1      |-----------------------|
 |--------------------o---------->|  id: string           |
 | quantity: unsigned |           |  name: string         |
 +--------------------+           |  description: string  |
                                  |  price: double        |
                                  +-----------------------+
```

Below are correponding object sizes in bytes:

```
Class       ICP  Conventional
--------   ----  ----
Customer     40   152
Product      24   112
Item          8    24
Order        40    80
```

# String API

```c++
#include <icp/IString.h>
...
icp::IString username("John");
```

then use it as a regular std::string.

# Pointers API

```c++
// MyClass.h
#include <icp/IPtr.h>
...
class MyClass
{
    ...
};

// not mandatory but convenient
typedef icp::Ptr<MyClass> MyClassPtr;
```

```c++
// MyClass.cpp
#include "<MyClass.h"

// required to instantiate template
#include <icp/Ptr.hpp>

// optional: override default pool count and size for MyClass only
template<> 
constexpr icp::ptr_type icp::Ptr<MyClass>::NoPoolsBits(){ return 12; };

// instantiate statics and methods
template class icp::Ptr<MyClass>;

```

See example in src/test/TestOrders.cpp
