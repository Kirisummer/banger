#include <iomanip>
#include <sstream>

#include "percent_encoder.hpp"

namespace {

uint8_t utf8_cp_size(uint8_t byte) {
    if (byte & 0b1000'0000) {
        if ((byte >> 5) == 0b110) {
            return 2;
        } else if ((byte >> 4) == 0b1110) {
            return 3;
        } else if ((byte >> 3) == 0b11110) {
            return 4;
        } else {
            return 0; // invalid code point
        }
    } else {
        return 1;
    }
}

bool is_special(char ch) {
    std::string_view special{"!#$&\"'()*+,/:;=?@[]"};
    return special.find(ch) != std::string_view::npos;
}

}  // namespace

namespace banger {

struct PercentEncoder::Impl {
    std::stringstream result_;

    void feed_code_point(std::string_view code_point);
    void feed_encode(char ch);
    void feed_literal(char ch);
};

PercentEncoder::PercentEncoder():
    impl_{std::make_unique<PercentEncoder::Impl>()}
{}

PercentEncoder::~PercentEncoder() = default;

zeus::expected<void, std::size_t> PercentEncoder::feed(std::string_view str) {
    std::size_t pos = 0;
    while (pos < str.size()) {
        char ch = str.at(pos);
        if (ch == ' ') {
            impl_->feed_literal('+');
            ++pos;
            continue;
        }

        uint8_t bytes = utf8_cp_size(ch);
        if (!bytes) {
            return zeus::unexpected(pos);
        }

        auto code_point = str.substr(pos, bytes);
        if (bytes != code_point.size()) {
            return zeus::unexpected(pos);
        }

        impl_->feed_code_point(str.substr(pos, bytes));
        pos += bytes;
    }

    return {};
}

void PercentEncoder::Impl::feed_code_point(std::string_view code_point) {
    if (code_point.size() == 1) {
        char ch = code_point.at(0);
        if (ch <= 0x1F || is_special(ch)) {
            feed_encode(ch);
        } else {
            feed_literal(ch);
        }
    } else {
        for (char byte : code_point) {
            feed_encode(byte);
        }
    }
}

void PercentEncoder::Impl::feed_encode(char ch) {
    uint16_t ch_int = static_cast<uint16_t>(ch & 0xFF);
    result_ << '%' << std::setw(2) << std::hex << std::uppercase << ch_int;
}

void PercentEncoder::Impl::feed_literal(char ch) {
    result_ << ch;
}

std::string PercentEncoder::get() const {
    return impl_->result_.str();
}

}  // namespace banger
