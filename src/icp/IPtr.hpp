#include <icp/IPtr.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <mutex>

namespace icp
{


template< typename T >
IPtrPool<T> IPtrPool<T>::pools[ MAX_POOLS ]={};

template< typename T >
std::atomic<unsigned> IPtrPool<T>::poolHead( initPool0() );

template< typename T >
bool IPtrPool<T>::debug = false;

template< typename T >
void IPtrPool<T>::init( ptr_type size )
{
    data.assign( size * TSize, 0 );
    head.store( 1 );
    if( debug ) std::cout << "IPtrPool<" << typeid(T).name() + 1 << ">.size = " << data.size() << std::endl;
}

template< typename T >
IPtrPool<T>::~IPtrPool()
{
    if( debug && ! data.empty() )
    {
        std::cout << "IPtrPool<" << typeid(T).name() + 1 << ">.head = " << head.load() << std::endl;
    }
}

template< typename T >
unsigned IPtrPool<T>::initPool0()
{
    IPtrPool<T>::pools[0].init( DATA_SIZE );
    return 0;
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
