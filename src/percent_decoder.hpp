#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <zeus/expected.hpp>

namespace banger {

class PercentDecoder {
  public:
    PercentDecoder(const PercentDecoder&) = delete;
    PercentDecoder& operator=(const PercentDecoder&) = delete;

    PercentDecoder(PercentDecoder&&) = delete;
    PercentDecoder& operator=(PercentDecoder&&) = delete;

    PercentDecoder();
    ~PercentDecoder();

    zeus::expected<void, std::size_t> feed(std::string_view str);
    std::string finish();

  private:
    struct Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace banger
