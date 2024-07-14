#include <string_view>
#include <iostream>

#include <tinytest.h>

#include "percent_encoder.hpp"

int test_empty(const char *name){
    banger::PercentEncoder pe;
    TINYTEST_EQUAL("", pe.get());
    return 1;
}

int test_no_special(const char *name) {
    banger::PercentEncoder pe;
    pe.feed("ASCIIascii");
    TINYTEST_EQUAL("ASCIIascii", pe.get());
    return 1;
}

int test_utf8(const char *name) {
    banger::PercentEncoder pe;
    pe.feed("123");
    pe.feed("👁👄👁");
    pe.feed("456");
    TINYTEST_EQUAL("123%F0%9F%91%81%F0%9F%91%84%F0%9F%91%81456", pe.get());
    return 1;
}

int test_special(const char *name) {
    banger::PercentEncoder pe;
    pe.feed("1!2#3$4&5\"6'7(8)a*b+c,d/e:f;g=i?h@j[k");
    TINYTEST_EQUAL("1%212%233%244%265%226%277%288%29a%2Ab%2Bc%2Cd%2Fe%3Af%3Bg%3Di%3Fh%40j%5Bk", pe.get());
    return 1;
}

int test_mix(const char *name) {
    banger::PercentEncoder pe;
    pe.feed("!gi котики \"tuxedo\"");
    TINYTEST_EQUAL("%21gi+%D0%BA%D0%BE%D1%82%D0%B8%D0%BA%D0%B8+%22tuxedo%22", pe.get());
    return 1;
}

TINYTEST_DECLARE_SUITE(PercentEncoderTest);
TINYTEST_START_SUITE(PercentEncoderTest);
    /*
    */
    TINYTEST_ADD_TEST(test_empty, NULL, NULL);
    TINYTEST_ADD_TEST(test_no_special, NULL, NULL);
    TINYTEST_ADD_TEST(test_utf8, NULL, NULL);
    TINYTEST_ADD_TEST(test_special, NULL, NULL);
    TINYTEST_ADD_TEST(test_mix, NULL, NULL);
TINYTEST_END_SUITE();
TINYTEST_MAIN_SINGLE_SUITE(PercentEncoderTest);
