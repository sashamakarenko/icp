#include <iostream>
#include <cstring>
#include <icp/IString.h>
#include <pe/Measurement.h>

extern const char * longString, *pi1000;

int main( int, char** )
{
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
    m.initialize( 30 );
    
    auto len = std::strlen( longString );

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        icp::IString c( longString, len );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        std::string c( longString, len );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

}
