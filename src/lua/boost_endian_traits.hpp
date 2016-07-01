#ifndef UUID_4412E558_283B_48F5_9CC2_8CF95EDFC6E5
#define UUID_4412E558_283B_48F5_9CC2_8CF95EDFC6E5
#pragma once

#include <boost/endian/arithmetic.hpp>
#include "type_traits.hpp"

namespace Neptools
{
namespace Lua
{

template <boost::endian::order Order, typename T, size_t N,
          boost::endian::align A>
struct IsBoostEndian<boost::endian::endian_arithmetic<Order, T, N, A>>
    : std::true_type {};

#define X(type)                                         \
    template<> struct TypeName<boost::endian::type>     \
    { static constexpr const char* TYPE_NAME = #type; }
X(little_uint8_t);
X(little_uint16_t);
X(little_uint32_t);
#undef X

}
}

#endif
