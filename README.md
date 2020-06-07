# icp - Immutable Compact Pointers
Using 64-bit pointers and modern std::string makes your objects quite large and increases cache misse probability.

For example a structure like this
```
struct Instrument
{
    std::string id;
    Book * book;
};
```

