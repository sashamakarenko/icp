
#include "OrdersExt.h"

#include <icp/IPtr.hpp>

template<> constexpr icp::ptr_type CustomerPtr::NoPoolsBits(){ return 20; };
template class icp::IPtr<Customer>;
template class icp::IPtrPool<Customer>;

template<> constexpr icp::ptr_type ProductPtr::NoPoolsBits(){ return 18; };
template class icp::IPtr<Product>;
template class icp::IPtrPool<Product>;

template<> constexpr icp::ptr_type ItemPtr::NoPoolsBits(){ return 16; };
template class icp::IPtr<Item>;
template class icp::IPtrPool<Item>;

template<> constexpr icp::ptr_type OrderPtr::NoPoolsBits(){ return 12; };
template class icp::IPtr<Order>;
template class icp::IPtrPool<Order>;

