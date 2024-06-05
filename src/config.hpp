#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>

#include "error.hpp"

namespace banger {

class Config {
  public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    Config(Config&&) noexcept;
    Config& operator=(Config&&) noexcept;

    ~Config();

    static Ret<Config> create(std::istream &istr);
    std::string_view get_default() const noexcept;
    std::optional<std::string_view>
        get_query(std::string_view bang) const noexcept;
  
  private:
    struct Impl;

    Config(std::unique_ptr<const Impl>) noexcept;
    std::unique_ptr<const Impl> impl_;
};

}  // namespace banger
