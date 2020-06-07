#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cstring>
#include <icp/IString.h>

extern const char * longString, *pi1000, *pi999;

int main( int, char** )
{
    std::cout << "sizeof std::string " << sizeof( std::string )  << std::endl;
    std::cout << "sizeof std::string_view " << sizeof( std::string_view )  << std::endl;
    std::cout << "sizeof icp::IString " << sizeof( icp::IString )  << std::endl;
    
    icp::IString a;
    icp::IString pi1( pi1000 );
    icp::IString b1( "blabla" );
    icp::IString pi11( pi1000 );
    icp::IString b2( "blabla" );

    const unsigned nbThreads = 16;
    std::vector<std::thread> threads;
    for( int i = 0; i < nbThreads; ++i )
    {
        threads.emplace_back( [&](){
            const icp::IString::len_type piLen = pi1.size();
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib( 1, piLen - 2 );
            unsigned n = 100000/nbThreads;
            for( int j = 0; j < n; ++j )
            {
                const char * ptr = pi1.data() + distrib(gen);
                icp::IString str( ptr );
                auto sptr = str.data();
                auto slen = str.size();
                if( std::strlen( sptr ) != slen || std::strlen( ptr ) != slen )
                {
                    std::cerr << "insertion error: " << slen << "\n";
                }
            }
        } );
    }

    for( auto & th : threads )
    {
        th.join();
    }

    icp::IString b3( "blabla", true );
    icp::IString pi2( pi999 );
    icp::IString anything( "dadadadada" );
    icp::IString pi3( pi999, true );
    std::cout << "b2-b1=" << ( b2.data() - b1.data() ) << std::endl;
    std::cout << "b3-b1=" << ( b3.data() - b1.data() ) << std::endl;
    std::cout << "pi11-pi1=" << ( pi11.data() - pi1.data() ) << std::endl;
    std::cout << "pi3-pi2=" << ( pi3.data() - pi2.data() ) << std::endl;
}
