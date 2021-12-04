#include <thread>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cstring>
#include <icp/IString.h>
#include <pe/Measurement.h>
#include "Helper.h"

// lazy
#include "IStringExt.cpp"

bool dbg = icp::IString::setDebug( true );

int main( int, char** )
{
    std::unordered_map< std::string_view, icp::IString > cache;

    icp::IString e1, e2;
    e1 = "";
    cache[ e1 ] = e1;

    e2 = "";
    cache[ e2 ] = e2;
    CHECK( cache.size, cache.size(), == 1 )

    icp::IString pi( pi1000 );
 
    const icp::IString::len_type piLen = pi.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib( 1, piLen - 2 );

    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::llCacheReadMisses );
    m.addEvent( pe::EventType::branchMisses );
    m.addEvent( pe::EventType::busCycles );
    m.addEvent( pe::EventType::l1dCacheReadMisses );
    m.addEvent( pe::EventType::l1iCacheReadMisses );
    m.addEvent( pe::EventType::memory );
    m.initialize( 100 );

    for( int j = 0; j < 100000; ++j )
    {
        const char * ptr = pi.data() + distrib(gen);
        std::string_view view{ ptr };
        if( cache.find( view ) == cache.end() )
        {
            icp::IString str( ptr );
            cache[ str ] = str;
        }
    }

    for( int j = 0; j < 100; ++j )
    {
        const char * ptr = pi.data() + distrib(gen);
        m.startCapture();
        std::string_view view{ ptr };
        if( cache.find( view ) == cache.end() )
        {
            icp::IString str( ptr );
            cache[ str ] = str;
        }
        m.stopCapture();
    }

    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    CHECK( cache.size, cache.size(), > 10 )

    // std::string equivalent

    std::unordered_map< std::string, int > strcache;
    for( int j = 0; j < 100000; ++j )
    {
        const char * ptr = pi.data() + distrib(gen);
        std::string str{ ptr };
        if( strcache.find( str ) == strcache.end() )
        {
            strcache[ str ] = 1;
        }
    }

    for( int j = 0; j < 100; ++j )
    {
        const char * ptr = pi.data() + distrib(gen);
        m.startCapture();
        std::string str{ ptr };
        if( strcache.find( str ) == strcache.end() )
        {
            strcache[ str ] = 1;
        }
        m.stopCapture();
    }

    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


}
