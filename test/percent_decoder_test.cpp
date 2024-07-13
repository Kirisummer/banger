#include <string_view>
#include <iostream>

#include <tinytest.h>

#include "percent_decoder.hpp"

int test_no_special(const char *name) {
    std::string_view data{"q=123"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL(data.data(), pd.finish());
    return 1;
}

int test_empty(const char *name) {
    banger::PercentDecoder pd;
    TINYTEST_EQUAL("", pd.finish());
    return 1;
}

int test_multiple_finish(const char *name) {
    std::string_view data{"a=1"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL(data, pd.finish());
    TINYTEST_EQUAL(data, pd.finish());
    TINYTEST_EQUAL(data, pd.finish());
    return 1;
}

int test_plus_space(const char *name) {
    std::string_view data{"++"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL("  ", pd.finish());
    return 1;
}

int test_cyrilic(const char *name) {
    std::string_view data{"%D0%B0%D0%B1%D0%B2%D0%B3%D2%91%D0%B4"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL("абвгґд", pd.finish());
    return 1;
}

int test_double_percent(const char *name) {
    std::string_view data{"%%32"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL("%2", pd.finish());
    return 1;
}

int test_part_percent(const char *name) {
    std::string_view data{"%1%32"};
    banger::PercentDecoder pd;

    TINYTEST_TRUE(pd.feed(data));
    TINYTEST_EQUAL("%12", pd.finish());
    return 1;
}

TINYTEST_DECLARE_SUITE(PercentDecoderTest);
TINYTEST_START_SUITE(PercentDecoderTest);
    /*
    */
    TINYTEST_ADD_TEST(test_no_special, NULL, NULL);
    TINYTEST_ADD_TEST(test_empty, NULL, NULL);
    TINYTEST_ADD_TEST(test_multiple_finish, NULL, NULL);
    TINYTEST_ADD_TEST(test_plus_space, NULL, NULL);
    TINYTEST_ADD_TEST(test_cyrilic, NULL, NULL);
    TINYTEST_ADD_TEST(test_double_percent, NULL, NULL);
    TINYTEST_ADD_TEST(test_part_percent, NULL, NULL);
TINYTEST_END_SUITE();
TINYTEST_MAIN_SINGLE_SUITE(PercentDecoderTest);
