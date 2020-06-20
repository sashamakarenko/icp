#pragma once

#ifndef PROTECT_IPTR_H
#define PROTECT_IPTR_H

#include <stdint.h>
#include <iosfwd>
#include <atomic>
#include <vector>

namespace icp
{

typedef uint32_t ptr_type;

// Immutable Pointer
template< typename T >
class IPtr
{
    public:

        static constexpr ptr_type NoPoolsBits(){ return 10; };

        IPtr() noexcept : _idx{0} {}
        
        template< typename A, typename... Types >
        explicit IPtr( const A & a, Types... args ): _idx{0}
        {
            make( a, args... );
        }

        template< typename... Types >
        T & make( Types... args )
        {
            return * new( _idx ? getPtr() : allocate( _idx ) ) T( args... );
        }

        template< typename... Types >
        static IPtr<T> newInstance( Types... args )
        {
            IPtr<T> p;
            p.make( args... );
            return p;
        }

        ~IPtr() noexcept
        {
            // _idx = 0;
        }

        void destroyObject() const
        {
            T * obj = getPtr();
            if( obj )
            {
                obj->~T();
            }
        }
    
        T * get() const noexcept { return getPtr(); }
        
        T * operator->() const noexcept { return getPtr(); }
        T & operator*() const noexcept { return *getPtr(); }

        operator const T & () const;
        
        const IPtr & operator = ( const T & t ){ make(t); return *this; }
         
    private:
        
        T * allocate( ptr_type & idx );
        
        T * getPtr() const noexcept;
        
        ptr_type _idx;
};

template< typename T >
class IPtrPool
{
    public:
        
        IPtrPool(): head(0), data() {}
        
        void init( ptr_type size );
        
        ~IPtrPool();

    private:
        
        constexpr static ptr_type MAX_POOLS_BITS = IPtr<T>::NoPoolsBits();
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
        static bool                    debug;
        
};

template< typename T >
inline T * IPtr<T>::getPtr() const noexcept
{
    if( __builtin_expect( _idx, true ) )
    {
        return IPtrPool<T>::getPtr( _idx );
    }
    else
    {
        return nullptr;
    }
}

}

template< typename T >
std::ostream & operator << ( std::ostream & os, const icp::IPtr<T> & ptr )
{
    return os << ptr.get();
}

#endif
