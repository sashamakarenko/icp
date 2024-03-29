#pragma once

#ifndef PROTECT_ISTRING_H
#define PROTECT_ISTRING_H

#include <cstdint>
#include <string>
#include <cstring>
#include <string_view>
#include <iosfwd>

namespace icp
{

typedef uint32_t ptr_type;

class IStrFake;
// Immutable String.
class IString
{
    public:
        
        typedef uint16_t len_type;
        constexpr static const len_type npos = 0xffff;
        
                 IString() noexcept : _idx{0} {};
                 IString( const IString & other ) noexcept = default;
                 IString( const char * str, len_type len, bool reuse );
                 IString( const char * str, len_type len ): IString( str, len, reuseAllStrings ) {};
                 IString( const char * str, std::size_t len ): IString( str, (len_type)len, reuseAllStrings ) {};
        explicit IString( const char * str, bool reuse = reuseAllStrings ): IString( str, npos, reuse ) {};
        explicit IString( const std::string & str, bool reuse = reuseAllStrings ): IString( str.c_str(), str.size(), reuse ) {}
        explicit IString( const std::string_view & str, bool reuse = reuseAllStrings ): IString( str.data(), str.size(), reuse ) {}
        explicit IString( const IStrFake & view );

        ~IString() noexcept { /* _idx = 0; */ }
    
        const char * c_str() const noexcept { return ptr(); }
        const char * data() const noexcept { return ptr(); }
        const std::string_view toStringView() const noexcept { return { c_str(), size() }; }
        operator std::string_view() const noexcept { return { c_str(), size() }; }
        std::string toString() const noexcept { return { c_str(), size() }; }

        len_type size() const noexcept;
        
        bool operator == ( const IString & other ) const; // to review
        bool operator == ( const char * str ) const;
        bool operator != ( const IString & other ) const { return ! ( *this == other ); }
        
        operator bool () const noexcept { return _idx; }
        int compare( const IString & other ) const;
        bool operator < ( const IString & other ) const { return compare( other ) < 0; }

        void assign( const char * str, len_type len = npos, bool reuse = false );       

        const IString & operator = ( const char * str ){ assign( str ); return *this; }

        static bool setDebug( bool on );

        static void setReuseAllStrings( bool reuse ){ reuseAllStrings = reuse; }

        static bool getReuseAllString(){ return reuseAllStrings; }

        static len_type decodeLength( const char * ptr ) { return ( ((uint16_t)(uint8_t)ptr[0]) << 8 ) | ( (uint16_t)(uint8_t)ptr[1] ); }

    protected:

        constexpr static ptr_type TMP = ptr_type(0) - 1;
        struct tmp_trait{};
        explicit IString( tmp_trait ): _idx{ TMP } {}

    private:

        char * ptr() const noexcept;

        static IString searchExisting( const char * str, len_type len );

        static bool reuseAllStrings;

        ptr_type _idx;
};

class IStrFake: public IString
{
    public:
        
        explicit IStrFake( const char * ptr ): IString( tmp_trait{} ), _ptr(ptr) {}
        explicit IStrFake( const std::string & str ): IString( tmp_trait{} ), _ptr( str.c_str() ) {}
    
        const char * getPtr() const { return _ptr; }

    private:

        const char * const _ptr;
};

inline IString::IString( const IStrFake & view ): IString( view.getPtr() )
{
}

class IStrView
{
    public:
        
        explicit IStrView( const IString & str ): _ptr( str.c_str() ) {}
    
        const char * c_str() const noexcept { return _ptr; }
        const char * data() const noexcept { return _ptr; }
        operator std::string_view() const noexcept { return { c_str(), size() }; }
        std::string toString() const noexcept { return { c_str(), size() }; }

        IString::len_type size() const noexcept { return _ptr == nullptr ? 0 : IString::decodeLength( _ptr - 2 ); }

    private:

        const char * const _ptr;
};


}

inline icp::IStrFake operator "" _isf( const char * ptr, long unsigned ){ return icp::IStrFake( ptr ); }
inline icp::IString  operator "" _istr( const char * ptr, long unsigned ){ return icp::IString( ptr ); }
inline icp::IString  operator "" _ristr( const char * ptr, long unsigned ){ return icp::IString( ptr, true ); }

namespace std
{
    inline std::string to_string( const icp::IString & istr ) noexcept 
    {
        return istr.toString();
    }

    inline std::string to_string( const icp::IStrView & istr ) noexcept 
    {
        return istr.toString();
    }
}


std::ostream & operator << ( std::ostream & os, const icp::IString & str );

std::ostream & operator << ( std::ostream & os, const icp::IStrView & str );

#endif
