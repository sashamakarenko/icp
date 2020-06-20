#include <iostream>
#include <iomanip>
#include "OrdersExt.h"

void displayOrder( OrderPtr ord )
{
    const Order & order = *ord;
    std::cout << "\norder id: " << order.id << " customer: " << order.customer->email << "\n";
    for( int i = 0; i < order.items.size(); ++i )
    {
        const Item & item = order.items[i].get();
        std::cout << "\t" << (i+1) << " " << item.product->id << " px: " << item.product->price << " qty: " << item.quantity << "\n";
    }
}

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
    CustomerPtr c1( "#customer1"_isv, "Bernard"_ristr, std::string( "Dupont" ), "b.dupont@gmail.com"_istr );

    std::cout << c1->displayName << ": " << c1->firstName << " " << c1->secondName << " " << c1->email << "\n";

    CustomerPtr c2;
    c2.make();
    c2->displayName = "#customer2";
    std::cout << "c2=c1: " << ( & c2.get() - & c1.get() ) << std::endl;

    ProductPtr gxy9  = ProductPtr::newInstance( "SGXY09", "Samsung Galaxy S9",  "Old phone...", 800 );
    ProductPtr gxy10 = ProductPtr::newInstance( "SGXY10", "Samsung Galaxy S10", "Quite a phone...", 900 );

    OrderPtr order = OrderPtr::newInstance();
    order->id = "0000001";
    order->customer = c1;
    order->items.emplace_back( gxy9, 2 );
    order->items.push_back( ItemPtr( gxy10 ) );
    c1->ordersHistory.push_back( order );
    displayOrder( order );

    c1.destroyObject();
}
