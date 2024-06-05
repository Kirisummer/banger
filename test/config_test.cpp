#include <iostream>
#include <sstream>
#include <string>

#include <tinytest.h>

#include "config.hpp"

int test_valid(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
bangs = ['alias1', 'alias2']\n\
query = 'query1'\n\
\n\
[[bangs]]\n\
bangs = ['аліас3']\n\
query = 'query2'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_ASSERT(config_res);
    auto &config = config_res.value();

    TINYTEST_EQUAL("alias1", config.get_default());
    TINYTEST_EQUAL("query1", config.get_query("alias1").value());
    TINYTEST_EQUAL("query1", config.get_query("alias2").value());
    TINYTEST_EQUAL("query2", config.get_query("аліас3").value());
    TINYTEST_ASSERT(!config.get_query("none"));

    return 1;
}

int test_valid_unicode_default(const char *name) {
    std::string data{
"default='аліас3'\n\
\n\
[[bangs]]\n\
bangs = ['alias1', 'alias2']\n\
query = 'query1'\n\
\n\
[[bangs]]\n\
bangs = ['аліас3']\n\
query = 'query2'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_ASSERT(config_res);
    auto &config = config_res.value();

    TINYTEST_EQUAL("аліас3", config.get_default());
    TINYTEST_EQUAL("query1", config.get_query("alias1").value());
    TINYTEST_EQUAL("query1", config.get_query("alias2").value());
    TINYTEST_EQUAL("query2", config.get_query("аліас3").value());
    TINYTEST_ASSERT(!config.get_query("none"));

    return 1;
}

int test_no_default(const char *name) {
    std::string data{
"[[bangs]]\n\
bangs = ['alias1', 'alias2']\n\
query = 'query1'\n\
\n\
[[bangs]]\n\
bangs = ['аліас3']\n\
query = 'query2'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_wrong_type_default(const char *name) {
    std::string data{
"default=0\n\
\n\
[[bangs]]\n\
bangs = ['alias1', 'alias2']\n\
query = 'query1'\n\
\n\
[[bangs]]\n\
bangs = ['аліас3']\n\
query = 'query2'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_no_bangs(const char *name) {
    std::string data{"default='аліас3'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_wrong_type_bangs(const char *name) {
    std::string data{ "default='аліас3'\nbangs=''"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_no_query(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
bangs = ['alias1', 'alias2']"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_wrong_type_query(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
query = 0\n\
bangs = ['alias1', 'alias2']"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_no_bangs(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
query = 'query1'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_bangs_not_array(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
query = 0\n\
bangs = 'alias1'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_bangs_empty(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
query = 0\n\
bangs = []"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_table_bangs_not_strings(const char *name) {
    std::string data{
"default='alias1'\n\
\n\
[[bangs]]\n\
query = 0\n\
bangs = [0, 1]"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}

int test_default_not_found(const char *name) {
    std::string data{
"default='alias4'\n\
\n\
[[bangs]]\n\
bangs = ['alias1', 'alias2']\n\
query = 'query1'\n\
\n\
[[bangs]]\n\
bangs = ['аліас3']\n\
query = 'query2'"};
    std::istringstream istr(data);

    auto config_res = banger::Config::create(istr);
    TINYTEST_FALSE(config_res);

    return 1;
}


TINYTEST_DECLARE_SUITE(ConfigTest);
TINYTEST_START_SUITE(ConfigTest);
    TINYTEST_ADD_TEST(test_valid, NULL, NULL);
    TINYTEST_ADD_TEST(test_valid_unicode_default, NULL, NULL);
    TINYTEST_ADD_TEST(test_no_default, NULL, NULL);
    TINYTEST_ADD_TEST(test_wrong_type_default, NULL, NULL);
    TINYTEST_ADD_TEST(test_no_bangs, NULL, NULL);
    TINYTEST_ADD_TEST(test_wrong_type_bangs, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_no_query, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_wrong_type_query, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_no_bangs, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_bangs_empty, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_bangs_not_array, NULL, NULL);
    TINYTEST_ADD_TEST(test_table_bangs_not_strings, NULL, NULL);
    TINYTEST_ADD_TEST(test_default_not_found, NULL, NULL);
    /*
    */
TINYTEST_END_SUITE();
TINYTEST_MAIN_SINGLE_SUITE(ConfigTest);
