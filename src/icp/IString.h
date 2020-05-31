#pragma once

#ifndef PROTECT_ISTRING_H
#define PROTECT_ISTRING_H

#include <stdint.h>
#include <string>
#include <iosfwd>

namespace icp
{

typedef uint32_t ptr_type;
    
class IString
{
    public:
        
        typedef uint16_t len_type;
        constexpr static const len_type npos = 0xffff;
        
        explicit IString() : _idx{0} {};
        explicit IString( const char * str, len_type len = npos );
        explicit IString( const std::string & str ): IString( str.c_str(), str.size() ) {}

        ~IString() { _idx = 0; }
    
        const char * c_str() const;
        const char * data() const { return c_str(); }
        
        len_type size() const;
        
        bool operator == ( const IString & other ) const;
        bool operator != ( const IString & other ) const { return ! ( *this == other ); }
        
        bool operator < ( const IString & other ) const;

        bool assign( const char * str, len_type len = npos );       

    private:
        
        ptr_type _idx;
};
    
}

std::ostream & operator << ( std::ostream & os, const icp::IString & str );

#endif
