#pragma once

#include <string>

#include <zeus/expected.hpp>

namespace banger {

// Error type. Contains a return value OR an error string
template <typename T>
using Ret = ::zeus::expected<T, std::string>;
using Err = ::zeus::unexpected<std::string>;

};
