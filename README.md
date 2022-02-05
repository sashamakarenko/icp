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

Below are corresponding object sizes in bytes:

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
## Caching

```c++
std::unordered_map< std::string_view, icp::IString > allUserNames;
...

// name comes from a network message for example
User::Ptr createUser( const char * name, size_t nameLength )
{
    // tip: lock allUserNames here if necessary
    std::string_view view{ name, nameLength };
    auto nameIt = allUserNames.find( view );
    if( it == allUserNames.end() )
    {
        // construct it really
        icp::IString newName( name, nameLength );

        // important to use newName as key and value
        nameIt = allUserNames.insert( newName, newName ).first;
    }
    return User::newInstance( nameIt->second );
}

```

## IStrFake class and zero copy use cases

## IStrView faster access wrapper

# Pointers API

### MyClass.h

```c++
#include <icp/IPtr.h>
...
class MyClass
{
    ...
};

// not mandatory but convenient
typedef icp::Ptr<MyClass> MyClassPtr;
```

### MyClass.cpp
```c++
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
