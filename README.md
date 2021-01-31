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
