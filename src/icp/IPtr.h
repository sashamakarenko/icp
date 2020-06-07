#pragma once

#ifndef PROTECT_IPTR_H
#define PROTECT_IPTR_H

#include <stdint.h>
#include <iosfwd>

namespace icp
{

typedef uint32_t ptr_type;

// Immutable Pointer
template< typename T >
class IPtr
{
    public:

        IPtr(): _idx{0} {}
        
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

        ~IPtr()
        {
            _idx = 0;
        }

        void destroyObject() const
        {
            T * obj = getPtr();
            if( obj )
            {
                obj->~T();
            }
        }
    
        T & get() const { return * getPtr(); }
        
        operator const T & () const;
        
        const IPtr & operator = ( const T & t ){ make(t); return *this; }
         
    private:
        
        T * allocate( ptr_type & idx );
        
        T * getPtr() const;
        
        ptr_type _idx;
};
    
}

template< typename T >
std::ostream & operator << ( std::ostream & os, const icp::IPtr<T> & ptr )
{
    return os << ptr.get();
}

#endif
