#include <thread>
#include <vector>
#include <set>
#include <random>
#include <cstring>
#include <icp/IString.h>
#include "Helper.h"

extern const char * longString, *pi1000, *pi999;

bool dbg = icp::IString::setDebug( true );

int main( int, char** )
{
    std::cout << "sizeof std::string " << sizeof( std::string )  << std::endl;
    std::cout << "sizeof std::string_view " << sizeof( std::string_view )  << std::endl;
    std::cout << "sizeof icp::IString " << sizeof( icp::IString )  << std::endl;
    
    // icp::IString::setReuseAllStrings( true );

    icp::IString e1, e2;
    e1 = "";
    e2 = "";
    CHECK( e2-e1, e2.data() - e1.data(), == 0 )

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
            unsigned n = 1000000/nbThreads;
            for( int j = 0; j < n; ++j )
            {
                icp::IString str( pi1.data() + distrib(gen), true );
            }
        } );
    }

    for( auto & th : threads )
    {
        th.join();
    }

    icp::IString b3( "blabla", true );
    CHECK( b3=blabla, b3 == "blabla", == true )
    CHECK( b2-b1, b2.data() - b1.data(), > 0 )
    CHECK( b3-b1, b3.data() - b1.data(), == 0 )

    icp::IString pi2( pi999 );
    icp::IString anything( "dadadadada" );
    icp::IString pi3( pi999, true );
    CHECK( pi11-pi1, pi11.data() - pi1.data(), != 0  )
    CHECK( pi3-pi2, pi3.data() - pi2.data(), == 0 )

    std::set<icp::IString> sset;
    sset.emplace( "123" );
    sset.emplace( "123", true );
    sset.emplace( "123" );
    CHECK( set.size, sset.size(), == 1 )
    sset.emplace( "234" );
    CHECK( set.size, sset.size(), == 2 )

    bool found = sset.find( icp::IStrView( "123" ) ) != sset.end();
    CHECK( found, found, == true )

    found = sset.find( "012"_isv) != sset.end();
    CHECK( found, found, == false )

    found = sset.find( (icp::IStrView)std::string( "012" ) ) != sset.end();
    CHECK( found, found, == false )
}
