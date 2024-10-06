#include <gtest/gtest.h>
#include "render.h"
#include "fix.h"

// Macros don't play well with curly-brace constructor
// calls and commas -> use parenthesis

TEST(Fix10, constr_int){
    ASSERT_EQ(Fix10( 123, PrescaledTag{}).val,  123);
    ASSERT_EQ(Fix10(-123, PrescaledTag{}).val, -123);
}

TEST(Fix10, constr_int2){
    ASSERT_EQ(Fix10{ 2}.val,  2048);
    ASSERT_EQ(Fix10{-2}.val, -2048);
}

TEST(Fix10, constr_float){
    ASSERT_EQ(Fix10{ 0.0f}.val,     0);
    ASSERT_EQ(Fix10{ 1.0f}.val,  1024);
    ASSERT_EQ(Fix10{ 0.5f}.val,   512);
    ASSERT_EQ(Fix10{-1.0f}.val, -1024);
    ASSERT_EQ(Fix10{-0.5f}.val,  -512);
}

TEST(Fix10, toFloat){
    ASSERT_EQ(Fix10(    0, PrescaledTag{}).toFloat(),  0.0f);
    ASSERT_EQ(Fix10(  512, PrescaledTag{}).toFloat(),  0.5f);
    ASSERT_EQ(Fix10( 1024, PrescaledTag{}).toFloat(),  1.0f);
    ASSERT_EQ(Fix10(-1024, PrescaledTag{}).toFloat(), -1.0f);
}

TEST(Fix10, add){
    ASSERT_EQ((Fix10{123}+Fix10{111}).val, 234);
}

TEST(Fix10, substract){
    ASSERT_EQ((Fix10{123}-Fix10{111}).val, 12);
}

TEST(Fix10, multiply){
    // 4*0.5
    ASSERT_EQ((Fix10{4096}*Fix10{512}).val, 2048);
}

TEST(Fix10, divide){
    // 2/0.5
    ASSERT_EQ((Fix10{2048}/Fix10{512}).val, 4096);
}

TEST(Fix10, ceil) {
    ASSERT_EQ(ceil(Fix10{   0}).val,    0);

    ASSERT_EQ(ceil(Fix10{   1}).val, 1024);
    ASSERT_EQ(ceil(Fix10{1023}).val, 1024);
    ASSERT_EQ(ceil(Fix10{1024}).val, 1024);
    ASSERT_EQ(ceil(Fix10{1025}).val, 2048);

    ASSERT_EQ(ceil(Fix10{   -1}).val, 0);
    ASSERT_EQ(ceil(Fix10{-1023}).val, 0);
    ASSERT_EQ(ceil(Fix10{-1024}).val, -1024);
    ASSERT_EQ(ceil(Fix10{-1025}).val, -1024);
}

