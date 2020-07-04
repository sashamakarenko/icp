
#include "OrdersExt.h"

#include <icp/Ptr.hpp>

template<> constexpr icp::ptr_type CustomerPtr::NoPoolsBits(){ return 20; };
template class icp::Ptr<Customer>;

template<> constexpr icp::ptr_type ProductPtr::NoPoolsBits(){ return 18; };
template class icp::Ptr<Product>;

template<> constexpr icp::ptr_type ItemPtr::NoPoolsBits(){ return 16; };
template class icp::Ptr<Item>;

template<> constexpr icp::ptr_type OrderPtr::NoPoolsBits(){ return 12; };
template class icp::Ptr<Order>;

double totalPrice;
