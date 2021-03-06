/**
* @brief 
* @file test_helpers.cpp
* @author J.H. 
* @date 2018-10-09
*/

/* system includes C */

/* system includes C++ */
#include <gtest/gtest.h>
#include <list>


/* local includes */
#include "helpers/hextobin.h"
#include "helpers/fp2str.h"

#define TEST_BUF_LEN    64


TEST(TEST_HELPERS, TEST_HEXTOBIN)
{
    const std::list<std::pair<const char*, const char*>> _test_data = {
        { "ff", "\xff" },
        { "00ff", "\x00\xff" },
        { "00ffaa11223344ee", "\x00\xff\xaa\x11\x22\x33\x44\xee" },
        { "0123456789abcdef", "\x01\x23\x45\x67\x89\xab\xcd\xef" },
        { "0123456789ABCDEF", "\x01\x23\x45\x67\x89\xab\xcd\xef" }
    };

    const std::list<std::pair<const char *, int>> _test_data_fail = {
        {"mf",  E_INVALID_CHAR},
        {"",    E_EMPTY_STRING},
        {"abC", E_INVALID_STRING_LEN},
        {"aBcDef1r",     E_INVALID_CHAR},
        {"abC.eF12",     E_INVALID_CHAR}
    };

    uint8_t buf[TEST_BUF_LEN];
    for (auto dta_p : _test_data) {
        int retlen = hextobin(dta_p.first, buf, TEST_BUF_LEN);
        ASSERT_GT(retlen, 0);
        ASSERT_EQ(memcmp(buf, dta_p.second, retlen), 0);
    }
    for (auto dta_p : _test_data_fail) {
        ASSERT_EQ(hextobin(dta_p.first, buf, TEST_BUF_LEN), dta_p.second);
    }
}

TEST(TEST_HELPERS, TEST_FP2STR)
{
    struct test_data {
        uint32_t val;
        uint8_t frac;
        const char *expected;
    };
    char buf[32];
    // test invalid buffer len
    ASSERT_EQ(fp2str(0b1111, 4, buf, 6), -1);
    ASSERT_EQ(fp2str(0b1111, 4, buf, 4), -1);
    ASSERT_EQ(fp2str(0b1111, 4, buf, 3), -1);

    // test weird cases
    ASSERT_EQ(fp2str(0b0, 4, buf, sizeof(buf)), 0);
    ASSERT_EQ(fp2str(INT32_MAX, 4, buf, sizeof(buf)), 0);

    // test good values
    const std::list<struct test_data> _test_data = {
        { UINT32_MAX, 4, "268435455.9375"},
        { 0b1101, 2, "3.25"},
        { 0b1100, 2, "3"},
        { 0b1111, 0, "15"},
        { 0b1111, 1, "7.5"},
        { 0b1111, 2, "3.75"},
        { 0b1111, 3, "1.875"},
        { 0b1111, 4, "0.9375"},
        { 0b0, 4, "0"},

    };
    for(auto dta_p : _test_data) {
        ASSERT_EQ(fp2str(dta_p.val, dta_p.frac, buf, sizeof(buf)), 0);
        ASSERT_EQ(strcmp(dta_p.expected, buf), 0);
    }

}
