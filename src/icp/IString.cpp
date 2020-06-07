#include <icp/IString.h>
#include <vector>
#include <atomic>
#include <iostream>
#include <cstring>
#include <exception>
#include <mutex>

namespace icp
{

namespace
{
    constexpr static ptr_type MAX_POOLS_BITS = 5;
    constexpr static ptr_type MAX_POOLS      = 1 << MAX_POOLS_BITS;

    constexpr static ptr_type DATA_SIZE_BITS = sizeof( ptr_type ) * 8 - MAX_POOLS_BITS;
    constexpr static ptr_type DATA_SIZE      = 1 << DATA_SIZE_BITS;
    constexpr static ptr_type DATA_MASK      = DATA_SIZE - 1;

std::mutex logMtx;

struct IStringPool
{
    IStringPool(): head(0), data() {}

    void init( ptr_type size );

    ~IStringPool();

    std::atomic< ptr_type > head;
    std::vector< char >     data;

    static std::atomic< unsigned > poolHead;
    static IStringPool             pools[ MAX_POOLS ];
        
};


void IStringPool::init( ptr_type size )
{
    data.assign( size, 0 );
    head = 2;
    std::lock_guard<std::mutex> guard( logMtx );
    std::cout << "IStringPool.size = " << data.size() << std::endl;
}

IStringPool::~IStringPool()
{
    if( ! data.empty() )
    {
        std::lock_guard<std::mutex> guard( logMtx );
        std::cout << "IStringPool.head = " << head.load() << std::endl;
    }
}


IStringPool pools[ MAX_POOLS ];

unsigned initPool0();
std::atomic<unsigned> poolHead( initPool0() );

unsigned initPool0()
{
    pools[0].init( DATA_SIZE );
    return 0;
}

}

/*
pool data layout

  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
|___|___|___|___|___|___|___|___|___|___|___|___|___|___|...
  0   0   3   A   B   C   0   1   2   V   E   R   Y  ... 
  |   |                       |
  |   string ABC idx          130 byte long string idx
  reserved for 0 length strings
*/

IString IString::searchExisting( const char * str, len_type len )
{
    IString ret;
    for( unsigned ph = 0; ph <= poolHead.load(); ++ph )
    {
        auto & pool = pools[ ph ];
        ptr_type endOfData = pool.head.load();
        for( ptr_type idx = 1; idx < endOfData; )
        {
            const char * ptr = & pool.data[ idx ];
            len_type l = ( ((uint16_t)(uint8_t)ptr[0]) << 8 ) | ( (uint16_t)(uint8_t)ptr[1] );
            if( l == 0 )
            {
                // corrupted data
                std::cerr << "corrupted data " << std::endl;
                break;
            }
            if( ptr[ l + 2 ] != 0 ) // long string > 255
            {
                ++ptr;
                ++idx;
                l = ( ((uint16_t)(uint8_t)ptr[0]) << 8 ) | ( (uint16_t)(uint8_t)ptr[1] );
                if( ptr[ l + 2 ] != 0 )
                {
                    // corrupted data
                    std::cerr << "corrupted data " << std::endl;
                    break;
                }
            }
            if( l == len && std::strncmp( ptr + 2, str, l ) == 0 )
            {
                ret._idx = ( ph << DATA_SIZE_BITS ) + idx;
                return ret;
            }
            idx += l + 2;
        }
    }
    return ret;
}


bool IString::assign( const char * str, IString::len_type len, bool reuse )
{
    if( _idx != 0 )
    {
        return false;
    }
    
    if( str == nullptr || len == 0 )
    {
        return true;
    }
    
    if( len == npos )
    {
        auto sz = ::strlen( str );
        if( sz >= npos )
        {
            throw std::logic_error( "IString: too long string" );
        }
        len = (len_type)sz;
    }
    
    if( reuse )
    {
        IString existing = searchExisting( str, len );
        if( existing._idx )
        {
            _idx = existing._idx;
            return true;
        }
    }

    unsigned lenSize = len < 255U ? 1 : 2;
    unsigned sz      = len + lenSize + 1;
    unsigned ph      = poolHead.load();
    unsigned idx;
    
    while( pools[ ph ].head.load() == 0 )
    {
    }
    
    idx = pools[ ph ].head.fetch_add( sz );
    
    while( idx + sz >= DATA_SIZE )
    {
        if( poolHead.compare_exchange_strong( ph, ph + 1 ) )
        {
            ++ph;
            pools[ ph ].init( DATA_SIZE );
        }
        else
        {
            while( pools[ ph ].head.load() == 0 )
            {
                // wait init() over
            }
        }
        
        if( ph >= MAX_POOLS )
        {
            throw std::logic_error( "IStringPool: no more pools available" );
        }
        idx = pools[ ph ].head.fetch_add( sz );
    }

    auto & pool = pools[ ph ];
    
    if( len < 255U )
    {
        --idx;
    }
    
    memcpy( & pool.data[ idx + 2 ], str, len );
    
    if( len < 255U )
    {
        pool.data[ idx + 1 ] = (uint8_t)len;
        if( pool.data[ idx ] != 0 )
        {
            throw std::logic_error( "IStringPool: corrupted short data buffer" );
        }
    }
    else
    {
        pool.data[ idx     ] = (uint8_t)( ( len >> 8 ) & 0xff );
        pool.data[ idx + 1 ] = (uint8_t)(          len & 0xff );
        if( pool.data[ idx - 1 ] != 0 )
        {
            throw std::logic_error( "IStringPool: corrupted long data buffer" );
        }
    }

    _idx = ( ph << DATA_SIZE_BITS ) + idx;
    return true;
}

IString::IString( const char * str, IString::len_type len, bool reuse ) : _idx{0}
{
    assign( str, len, reuse );
}

char * IString::ptr() const noexcept
{
    return _idx ? & pools[ _idx >> DATA_SIZE_BITS ].data[ ( _idx & DATA_MASK ) + 2 ] : nullptr;
}

IString::len_type IString::size() const noexcept
{
    if( _idx == 0 )
    {
        return 0;
    }
    
    const char * ptr = & pools[ _idx >> DATA_SIZE_BITS ].data[ _idx & DATA_MASK ];
    return ( ((uint16_t)(uint8_t)ptr[0]) << 8 ) | ( (uint16_t)(uint8_t)ptr[1] );
}

bool IString::operator == ( const IString & other ) const
{
    if( _idx == other._idx )
    {
        return true;
    }
    return strcmp( c_str(), other.c_str() ) < 0;
}

}

std::ostream & operator << ( std::ostream & os, const icp::IString & str )
{
    return os << str.c_str();
}
