#pragma once

#include <memory>
#include <string_view>
#include <string>

#include <zeus/expected.hpp>

namespace banger {

class PercentEncoder {
  public:
    PercentEncoder(const PercentEncoder&) = delete;
    PercentEncoder& operator=(const PercentEncoder&) = delete;

    PercentEncoder(PercentEncoder&&) = delete;
    PercentEncoder& operator=(PercentEncoder&&) = delete;

    PercentEncoder();
    ~PercentEncoder();

    zeus::expected<void, std::size_t> feed(std::string_view str);
    std::string get() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace banger
