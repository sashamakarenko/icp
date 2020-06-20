#ifndef ORDERS_EXT_H
#define ORDERS_EXT_H

#include <icp/IPtr.h>
#include <icp/IString.h>
#include <iostream>
#include <vector>
#include <memory>

struct Order;

typedef icp::IPtr<Order> OrderPtr;

struct Customer
{
    Customer() = default; 

    template< typename TDN, typename TFN, typename TSN, typename TEM >
    Customer( TDN dn, TFN fn, TSN sn, TEM em ):
        displayName{dn},
        firstName{fn},
        secondName{sn},
        email{em}
    {
    }

    icp::IString          displayName;
    icp::IString          firstName;
    icp::IString          secondName;
    icp::IString          email;
    std::vector<OrderPtr> ordersHistory;
};

typedef icp::IPtr<Customer> CustomerPtr;

struct Product
{
    Product() = default; 

    template< typename TID, typename TNM, typename TDS >
    Product( TID id, TNM nm, TDS ds, double px ):
        id{id},
        name{nm},
        description{ds},
        price{px}
    {
    }

    icp::IString         id;
    icp::IString         name;
    icp::IString         description;
    double               price;
};

typedef icp::IPtr<Product> ProductPtr;

struct Item
{
    Item() = default;
    Item( ProductPtr p, uint16_t qty = 1 ): product{p}, quantity{qty} {}

    ProductPtr           product;
    uint16_t             quantity;
};

typedef icp::IPtr<Item> ItemPtr;


struct Order
{
    enum class State: uint8_t
    {
        New,
        Payed,
        Delivering,
        Deliverred,
        Cancelled
    };
    icp::IString         id;
    CustomerPtr          customer;
    std::vector<ItemPtr> items;
    State                state = State::New;
};

// ------------------------------------------------- conventional implementation

namespace conv
{

struct Order;
typedef std::shared_ptr<Order> OrderPtr;

struct Customer
{
    std::string           displayName;
    std::string           firstName;
    std::string           secondName;
    std::string           email;
    std::vector<OrderPtr> ordersHistory;
};

typedef std::shared_ptr<Customer> CustomerPtr;

struct Product
{
    std::string          id;
    std::string          name;
    std::string          description;
    unsigned             quantity = 1;
    double               price;
};

typedef std::shared_ptr<Product> ProductPtr;

struct Item
{
    ProductPtr           product;
    uint16_t             quantity;
};

typedef std::shared_ptr<Item> ItemPtr;

struct Order
{
    enum class State: uint8_t
    {
        New,
        Payed,
        Delivering,
        Deliverred,
        Cancelled
    };
    std::string          id;
    CustomerPtr          customer;
    std::vector<ItemPtr> items;
    State                state = State::New;
};

}

#endif /* ORDERS_EXT_H */

