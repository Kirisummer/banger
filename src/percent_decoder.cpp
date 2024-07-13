#include "percent_decoder.hpp"

#include <cctype>
#include <optional>
#include <iostream>

namespace {

static char nibbles_to_byte(char first, char second) noexcept {
    return (first << 4) | second;
}

static char hex_to_nibble(char hex) noexcept {
    if (std::isdigit(hex)) {
        return hex - '0';
    }

    if (std::isupper(hex)) {
        return hex - 'A' + 10;
    }

    if (std::islower(hex)) {
        return hex - 'a' + 10;
    }

    return static_cast<char>(0);
}

}  // namespace

namespace banger {

struct PercentDecoder::Impl {
    std::string result_;
    bool finished_;
    bool percent_;
    std::optional<char> prev_xdigit_;

    bool feed(char ch);
    void feed_percent(char ch);
    void feed_regular(char ch);
    void flush_percent();
};

PercentDecoder::PercentDecoder():
    impl_{std::make_unique<PercentDecoder::Impl>()}
{}

PercentDecoder::~PercentDecoder() = default;

zeus::expected<void, std::size_t>
PercentDecoder::feed(std::string_view str) {
    for (std::size_t pos = 0; pos < str.size(); ++pos) {
        if (!impl_->feed(str.at(pos))) {
            return zeus::unexpected(pos);
        }
    }

    return {};
}

bool PercentDecoder::Impl::feed(char ch) {
    if (ch & static_cast<char>(0x80)) {
        return false;
    }

    if (percent_) {
        feed_percent(ch);
    } else {
        feed_regular(ch);
    }

    return true;
}

void PercentDecoder::Impl::feed_percent(char ch) {
    if (std::isxdigit(ch)) {
        if (!prev_xdigit_) {
            prev_xdigit_ = ch;
        } else {
            char first_nib = hex_to_nibble(prev_xdigit_.value());
            char second_nib = hex_to_nibble(ch);
            char from_hex = nibbles_to_byte(first_nib, second_nib);
            result_ += from_hex;
            prev_xdigit_ = std::nullopt;
            percent_ = false;
        }
    } else {
        feed_regular(ch);
    }
}

void PercentDecoder::Impl::flush_percent() {
    if (percent_) {
        result_ += '%';
        percent_ = false;
    }

    if (prev_xdigit_) {
        result_ += prev_xdigit_.value();
        prev_xdigit_ = std::nullopt;
    }
}

void PercentDecoder::Impl::feed_regular(char ch) {
    flush_percent();
    if (ch == '%') {
        percent_ = true;
    } else if (ch == '+') {
        result_ += ' ';
    } else {
        result_ += ch;
    }
}

std::string PercentDecoder::finish() {
    if (!impl_->finished_) {
        impl_->flush_percent();
        impl_->finished_ = true;
    }

    return impl_->result_;
}

}  // namespace banger
