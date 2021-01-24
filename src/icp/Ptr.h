#pragma once

#ifndef PROTECT_PTR_H
#define PROTECT_PTR_H

#include <stdint.h>
#include <iosfwd>
#include <atomic>
#include <vector>

namespace icp
{

typedef uint32_t ptr_type;

// Compact Pointer
template< typename T >
class Ptr
{
    public:

        static constexpr ptr_type NoPoolsBits(){ return 10; };

        Ptr() noexcept : _idx{0} {}
        
        template< typename A, typename... Types >
        explicit Ptr( const A & a, Types... args ): _idx{0}
        {
            construct( a, args... );
        }

        template< typename... Types >
        T & construct( Types... args )
        {
            return * new( _idx ? getPtr() : allocate( _idx ) ) T( args... );
        }

        template< typename... Types >
        static Ptr<T> newInstance( Types... args )
        {
            Ptr<T> p;
            p.construct( args... );
            return p;
        }

        ~Ptr() noexcept
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
        
        const Ptr & operator = ( const T & t ){ construct(t); return *this; }
    
    protected:

        ptr_type & getIndex() noexcept { return _idx; }
        void setIndex( ptr_type idx ) noexcept { _idx = idx; }

    private:
        
        T * allocate( ptr_type & idx );
        
        T * getPtr() const noexcept;
        
        ptr_type _idx;

        struct Pool
        {
                
            Pool(): head(0), data() {}
            
            void init( ptr_type size );
            
            ~Pool();

            constexpr static ptr_type MAX_POOLS_BITS = Ptr<T>::NoPoolsBits();
            constexpr static ptr_type MAX_POOLS      = 1 << MAX_POOLS_BITS;
            constexpr static ptr_type DATA_SIZE_BITS = sizeof( ptr_type ) * 8 - MAX_POOLS_BITS;
            constexpr static ptr_type DATA_SIZE      = 1 << DATA_SIZE_BITS;
            constexpr static ptr_type DATA_MASK      = DATA_SIZE - 1;
            
            constexpr static auto TSize = sizeof( T );

            static T * getPtr( ptr_type idx )
            {
                return reinterpret_cast< T * >( & pools[ idx >> DATA_SIZE_BITS ].data[ ( idx & DATA_MASK ) * TSize ] );
            }

            static T * allocate( ptr_type & idx );
            
            static unsigned initPool0();

            std::atomic< ptr_type > head;
            std::vector< char >     data;
            
            static Pool                    pools[ MAX_POOLS ];
            static std::atomic< unsigned > poolHead;
            static bool                    debug;
            
        };

 
};

template< typename T >
inline T * Ptr<T>::getPtr() const noexcept
{
    if( __builtin_expect( _idx, true ) )
    {
        return Ptr<T>::Pool::getPtr( _idx );
    }
    else
    {
        return nullptr;
    }
}

}

template< typename T >
std::ostream & operator << ( std::ostream & os, const icp::Ptr<T> & ptr )
{
    return os << ptr.get();
}

#endif
