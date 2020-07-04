#include <icp/Ptr.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <mutex>
#include <cxxabi.h>
#include <memory>

namespace icp
{

template< typename T >
std::string typeName()
{
    const char * name = typeid(T).name();
    int status;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle( name, nullptr, nullptr, &status),
        std::free
    };

    return status == 0  ? res.get() : name;
}

template< typename T >
typename Ptr<T>::Pool Ptr<T>::Pool::pools[ MAX_POOLS ]={};

template< typename T >
std::atomic<unsigned> Ptr<T>::Pool::poolHead( initPool0() );

template< typename T >
bool Ptr<T>::Pool::debug = true;

template< typename T >
void Ptr<T>::Pool::init( ptr_type size )
{
    data.assign( size * TSize, 0 );
    head.store( 1 );
    if( debug ) std::cout << "PtrPool<" << typeName<T>() << ">.size = " << data.size() << std::endl;
}

template< typename T >
Ptr<T>::Pool::~Pool()
{
    if( debug && ! data.empty() )
    {
        std::cout << "PtrPool<" << typeName<T>() << ">.head = " << head.load() << std::endl;
    }
}

template< typename T >
unsigned Ptr<T>::Pool::initPool0()
{
    Ptr<T>::Pool::pools[0].init( DATA_SIZE );
    return 0;
}


template< typename T >
T * Ptr<T>::Pool::allocate( ptr_type& idx )
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
            throw std::logic_error( "PtrPool<T>: no more pools available" );
        }
        idx = pools[ ph ].head.fetch_add( 1 );
    }

    auto & pool = pools[ ph ];
    
    T * ptr = reinterpret_cast< T* >( & pool.data[ idx * TSize ] );
    idx += ( ph << DATA_SIZE_BITS );
    
    return ptr;
}

template< typename T >
T * Ptr<T>::allocate( ptr_type & idx )
{
    return Ptr<T>::Pool::allocate( idx );
}

}
