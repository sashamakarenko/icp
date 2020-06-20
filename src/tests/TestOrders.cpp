#include <iostream>
#include <iomanip>
#include "OrdersExt.h"
#include <pe/Measurement.h>

template< typename OrderPtrT >
void displayOrder( OrderPtrT ord, std::ostream & out )
{
    const auto & order = *ord.get();
    out << "\norder id: " << order.id << " customer: " << order.customer->email << "\n";
    for( int i = 0; i < order.items.size(); ++i )
    {
        const auto & item = * order.items[i].get();
        out << "\t" << (i+1) << " " << item.product->id << " px: " << item.product->price << " qty: " << item.quantity << "\n";
    }
}

template< typename OrderPtrT >
double getOrderTotalPrice( OrderPtrT ord )
{
    double total = 0;
    for( auto & i : ord->items )
    {
        auto & item = *i;
        total += item.product->price * item.quantity;
    }
    return total;
}

extern volatile double totalPrice;

int main( int, char** )
{

    auto colw = std::setw(6);
    std::cout << "\nSizes:\n";
    std::cout << "Object      ICP  Conventional\n"
              << "--------   ----  ----\n"
              << "Customer " << colw << sizeof( Customer ) << colw << sizeof( conv::Customer ) << "\n"
              << "Product  " << colw << sizeof( Product  ) << colw << sizeof( conv::Product  ) << "\n"
              << "Item     " << colw << sizeof( Item     ) << colw << sizeof( conv::Item     ) << "\n"
              << "Order    " << colw << sizeof( Order    ) << colw << sizeof( conv::Order    ) << "\n\n";

    // icp::IString::setDebug( true );
    CustomerPtr c1( "#customer1", "Bernard"_ristr, std::string( "Dupont" ), "b.dupont@gmail.com"_istr );
    CustomerPtr c2;
    c2.make();
    c2->displayName = "#customer2";
    std::cout << "c2-c1 = " << ( c2.get() - c1.get() ) << std::endl;
    ProductPtr gxy9( "SGXY09", "Samsung Galaxy S9",  "Old phone...", 800 );
    ProductPtr gxy10( "SGXY10", "Samsung Galaxy S10", "Quite a phone...", 900 );

    std::cout << '\n' << c1->displayName << ": " << c1->firstName << " " << c1->secondName << " " << c1->email << " orders: " << c1->ordersHistory.size() << "\n";

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
    m.initialize( 20 );
    
    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        OrderPtr ord = OrderPtr::newInstance();
        Order & order = *ord;
        order.id = "0000001";
        order.customer = c1;
        order.items.emplace_back( gxy9, 2 );
        order.items.emplace_back( gxy10 );
        c1->ordersHistory.push_back( ord );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    conv::ProductPtr cp1 = std::make_shared<conv::Product>("SGXY09", "Samsung Galaxy S9",  "Old phone...", 800);
    conv::ProductPtr cp2 = std::make_shared<conv::Product>("SGXY10", "Samsung Galaxy S10", "Quite a phone...", 900 );
    conv::CustomerPtr cc1 = std::make_shared<conv::Customer>( "#customer1", "Bernard", "Dupont", "b.dupont@gmail.com" );

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        conv::OrderPtr cord = std::make_shared<conv::Order>();
        cord->id = "0000001";
        conv::Order & order = *cord.get();
        cord->customer = cc1;
        cord->items.emplace_back( std::make_shared<conv::Item>( cp1, 2 ) );
        cord->items.emplace_back( std::make_shared<conv::Item>( cp2 ) );
        cc1->ordersHistory.push_back( cord );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "order price: " << totalPrice << '\n';

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        totalPrice = getOrderTotalPrice( c1->ordersHistory[0] );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "order price: " << totalPrice << '\n';

    for( int j = 0; j < m.getMaxCaptures(); ++j )
    {
        m.startCapture();
        totalPrice = getOrderTotalPrice( cc1->ordersHistory[0] );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "order price: " << totalPrice << '\n';

}
