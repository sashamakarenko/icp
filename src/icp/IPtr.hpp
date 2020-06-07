#include <icp/IPtr.h>
#include <vector>
#include <atomic>
#include <iostream>
#include <cstring>
#include <exception>
#include <mutex>

namespace icp
{

template< typename T >
class IPtrPool
{
    public:
        
        IPtrPool(): head(0), data() {}
        
        void init( ptr_type size );
        
        ~IPtrPool();

    private:
        
        constexpr static ptr_type MAX_POOLS_BITS = 5;

        constexpr static ptr_type MAX_POOLS      = 1 << MAX_POOLS_BITS;

        constexpr static ptr_type DATA_SIZE_BITS = sizeof( ptr_type ) * 8 - MAX_POOLS_BITS;
        constexpr static ptr_type DATA_SIZE      = 1 << DATA_SIZE_BITS;
        constexpr static ptr_type DATA_MASK      = DATA_SIZE - 1;
        
        constexpr static auto TSize = sizeof( T );

    public:
    
        static T * getPtr( ptr_type idx )
        {
            return reinterpret_cast< T * >( & pools[ idx >> DATA_SIZE_BITS ].data[ ( idx & DATA_MASK ) * TSize ] );
        }

        static T * allocate( ptr_type & idx );
        
        static unsigned initPool0();

    private:
        
        std::atomic< ptr_type > head;
        std::vector< char >     data;
        
        static IPtrPool                pools[ MAX_POOLS ];
        static std::atomic< unsigned > poolHead;
        
};

template< typename T >
IPtrPool<T> IPtrPool<T>::pools[ MAX_POOLS ]={};

template< typename T >
std::atomic<unsigned> IPtrPool<T>::poolHead( initPool0() );

template< typename T >
void IPtrPool<T>::init( ptr_type size )
{
    data.assign( size * TSize, 0 );
    head.store( 1 );
    std::cout << "IPtrPool.size = " << data.size() << std::endl;
}

template< typename T >
IPtrPool<T>::~IPtrPool()
{
    if( ! data.empty() )
    {
        std::cout << "IPtrPool.head = " << head.load() << std::endl;
    }
}

template< typename T >
unsigned IPtrPool<T>::initPool0()
{
    IPtrPool<T>::pools[0].init( DATA_SIZE );
    return 0;
}


template< typename T >
T * IPtr<T>::getPtr() const
{
    if( _idx == 0 )
    {
        return nullptr;
    }
    T * ptr = IPtrPool<T>::getPtr( _idx );
    return ptr;
}

template< typename T >
T * IPtrPool<T>::allocate( ptr_type& idx )
{
    idx = 0;
    unsigned ph = poolHead.load();
    
    while( pools[ ph ].head.load() == 0 )
    {
    }
    
    idx = pools[ ph ].head.fetch_add( 1 );
    
    while( idx + 1 > DATA_SIZE )
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
            throw std::logic_error( "IPtrPool<T>: no more pools available" );
        }
        idx = pools[ ph ].head.fetch_add( 1 );
    }

    auto & pool = pools[ ph ];
    
    T * ptr = reinterpret_cast< T* >( & pool.data[ idx * TSize ] );
    idx += ( ph << DATA_SIZE_BITS );
    
    return ptr;
}

template< typename T >
T * IPtr<T>::allocate( ptr_type & idx )
{
    return IPtrPool<T>::allocate( idx );
}

}
