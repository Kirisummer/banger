#include <format>
#include <istream>
#include <string>
#include <string_view>
#include <unordered_map>

#include <toml++/toml.hpp>

#include "config.hpp"

namespace {

// Convert TOML node to string
template <typename Node>
inline std::string to_string(const Node &node) {
    std::ostringstream oss;
    oss << toml::node_view(node);
    return oss.str();
}

struct StringHash {
    // Allows unordered_map to search with string_view while having string key
    // taken from https://www.cppstories.com/2021/heterogeneous-access-cpp20/

    using is_transparent = void;

    [[nodiscard]] size_t operator()(const std::string &str) const {
        return std::hash<std::string>{}(str);
    }

    [[nodiscard]] size_t operator()(std::string_view view) const {
        return std::hash<std::string_view>{}(view);
    }
};

}  // namespace

namespace banger {

struct Config::Impl {
    std::string default_bang;
    std::unordered_map<std::string, std::string,
                       StringHash, std::equal_to<>> query_map;
};

Config::Config(Config&&) noexcept = default;
Config& Config::operator=(Config&&) noexcept = default;
Config::~Config() = default;

Ret<Config>
Config::create(std::istream &istr) {
    // Config parsing
    toml::parse_result parse_res = toml::parse(istr);
    if (!parse_res) {
        return Err(std::string(parse_res.error().description()));
    }
    toml::table config = std::move(parse_res.table());

    // Get default bang
    auto default_res = config["default"].value<std::string>();
    if (!default_res) {
        return Err("\"default\" is not present or is not a string");
    }
    std::string default_bang = std::move(default_res.value());

    // Get bang table array
    auto bang_arr_p = config["bangs"].as_array();
    if (!bang_arr_p) {
        return Err("\"bangs\" is not present or is not an array");
    }

    bool default_found = false;
    // I don't want to type all that...
    decltype(Config::Impl::query_map) query_map;

    for (toml::node &table_node : *bang_arr_p) {
        auto table_p = table_node.as_table();
        if (!table_p) {
            return Err("\"bangs\" contains a non-table node: "
                    + to_string(table_node));
        }
        auto &bang_table = *table_p;

        auto query_res = bang_table["query"].value<std::string>();
        if (!query_res) {
            return Err(
                   "\"query\" is not present in bang table or is not a string: "
                   + to_string(bang_table));
        }
        std::string query{std::move(query_res.value())};

        auto bangs_p = bang_table["bangs"].as_array();
        if (!bangs_p) {
            return Err(
                   "\"bangs\" is not present in bang table or is not a string: "
                   + to_string(bang_table));
        }

        if (bangs_p->empty()) {
            return Err(
                    "\"bangs\" is empty: " + to_string(bang_table));
        }
        for (toml::node &alias_node : *bangs_p) {
            auto alias_res = alias_node.value<std::string>();
            if (!alias_res) {
                return Err("Bang alias is not a string: "
                        + to_string(alias_node));
            }
            auto &alias = alias_res.value();
            if (!default_found && alias == default_bang) {
                default_found = true;
            }
            query_map.emplace( std::make_pair(std::move(alias), query));
        }
    }

    if (!default_found) {
        return Err(std::format("Default bang {} was not found in config",
                    default_bang));
    }

    auto impl = std::make_unique<const Config::Impl>(
            std::move(default_bang),
            std::move(query_map));

    return Config(std::move(impl));
}

Config::Config(std::unique_ptr<const Config::Impl> impl) noexcept:
    impl_{std::move(impl)}
{}

std::string_view Config::get_default() const noexcept {
    return impl_->default_bang;
}

std::optional<std::string_view> Config::get_query(std::string_view bang)
const noexcept {
    auto it = impl_->query_map.find(bang);
    if (it == impl_->query_map.end()) {
        return std::nullopt;
    }
    return it->second;
}

}  // namespace banger
