#pragma once

#ifndef PROTECT_ISTRING_H
#define PROTECT_ISTRING_H

#include <stdint.h>
#include <string>
#include <string_view>
#include <iosfwd>

namespace icp
{

typedef uint32_t ptr_type;

// Immutable String.
class IString
{
    public:
        
        typedef uint16_t len_type;
        constexpr static const len_type npos = 0xffff;
        
        explicit IString() noexcept : _idx{0} {};
                 IString( const IString & other ) noexcept = default;
        explicit IString( const char * str, len_type len, bool reuse = false );
        explicit IString( const char * str, bool reuse = false ): IString( str, npos, reuse ) {};
        explicit IString( const std::string & str, bool reuse = false ): IString( str.c_str(), str.size(), reuse ) {}
        explicit IString( const std::string_view & str, bool reuse = false ): IString( str.data(), str.size(), reuse ) {}

        ~IString() noexcept { _idx = 0; }
    
        const char * c_str() const noexcept { return ptr(); }
        const char * data() const noexcept { return ptr(); }
        const std::string_view toStringView() const noexcept { return { c_str(), size() }; }
        std::string toString() const noexcept { return { c_str(), size() }; }

        len_type size() const noexcept;
        
        bool operator == ( const IString & other ) const;
        bool operator != ( const IString & other ) const { return ! ( *this == other ); }
        
        bool operator < ( const IString & other ) const;
        operator bool () const noexcept { return _idx; };

        bool assign( const char * str, len_type len = npos, bool reuse = false );       

        const IString & operator = ( const char * str ){ assign( str ); return *this; }

    private:

        char * ptr() const noexcept;

        static IString searchExisting( const char * str, len_type len );

        ptr_type _idx;
};
    
}

namespace std
{
    std::string to_string( const icp::IString & istr ) noexcept 
    {
        return istr.toString();
    }
}

std::ostream & operator << ( std::ostream & os, const icp::IString & str );

#endif
