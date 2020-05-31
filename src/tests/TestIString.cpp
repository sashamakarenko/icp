#include <iostream>
#include <icp/IString.h>
#include <pe/Measurement.h>

int main( int, char** )
{
    icp::IString a;
    icp::IString b( "b" );
    std::cout << b << std::endl;

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
    
    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        icp::IString c( "012345678901234567890123456789012345678901234567890123456789" );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        std::string c( "012345678901234567890123456789012345678901234567890123456789" );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

}