
#include "PtrExt.h"

#include <icp/Ptr.hpp>

template<> 
constexpr icp::ptr_type icp::Ptr<S>::NoPoolsBits(){ return 12; };

template class icp::Ptr<S>;

