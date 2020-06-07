#include <iostream>
#include <icp/IString.h>

extern const char * longString, *pi1000;

int main( int, char** )
{
    std::cout << "sizeof std::string " << sizeof( std::string )  << std::endl;
    std::cout << "sizeof std::string_view " << sizeof( std::string_view )  << std::endl;
    std::cout << "sizeof icp::IString " << sizeof( icp::IString )  << std::endl;
    
    icp::IString a;
    icp::IString pi1( pi1000 );
    icp::IString b1( "blabla" );
    icp::IString piMiddle( pi1000 );
    icp::IString b2( "blabla" );
    icp::IString b3( "blabla", true );
    icp::IString pi2( pi1000 );
    icp::IString anything( "dadadadada" );
    icp::IString pi3( pi1000, true );
    std::cout << "b2-b1=" << ( b2.data() - b1.data() ) << std::endl;
    std::cout << "b3-b1=" << ( b3.data() - b1.data() ) << std::endl;
    std::cout << "pi2-pi1=" << ( pi2.data() - pi1.data() ) << std::endl;
    std::cout << "pi3-pi1=" << ( pi3.data() - pi1.data() ) << std::endl;
}
