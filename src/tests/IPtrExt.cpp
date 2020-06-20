
#include "IPtrExt.h"

#include <icp/IPtr.hpp>

template<> 
constexpr icp::ptr_type icp::IPtr<S>::NoPoolsBits(){ return 12; };

template class icp::IPtr<S>;

template class icp::IPtrPool<S>;

