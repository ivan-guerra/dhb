#include "base_conversions/base_conversions.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

/* testing with a number outside the range of an unsigned 64-bit integer to
 * check we can handle "big numbers" (number used below is 2^64 * 42)*/
static const std::string kHexNum("2A0000000000000000");
static const std::string kBinNum(
    "1010100000000000000000000000000000000000000000000000000000000000000000");
static const std::string kDecNum("774763251095801167872");
static const std::string kOctNum("124000000000000000000000");

TEST(BaseConvTests, ConvertBaseReturnsDecFromHex) {
    ASSERT_EQ(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kHex, dhb::NumSystem::kDec),
        kDecNum);
}

TEST(BaseConvTests, ConvertBaseReturnsDecFromBin) {
    ASSERT_EQ(
        dhb::ConvertBase(kBinNum, dhb::NumSystem::kBin, dhb::NumSystem::kDec),
        kDecNum);
}

TEST(BaseConvTests, ConvertBaseReturnsDecFromOct) {
    ASSERT_EQ(
        dhb::ConvertBase(kOctNum, dhb::NumSystem::kOct, dhb::NumSystem::kDec),
        kDecNum);
}

TEST(BaseConvTests, ConvertBaseReturnsBinFromDec) {
    ASSERT_EQ(
        dhb::ConvertBase(kDecNum, dhb::NumSystem::kDec, dhb::NumSystem::kBin),
        kBinNum);
}

TEST(BaseConvTests, ConvertBaseReturnsBinFromHex) {
    ASSERT_EQ(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kHex, dhb::NumSystem::kBin),
        kBinNum);
}

TEST(BaseConvTests, ConvertBaseReturnsBinFromOct) {
    ASSERT_EQ(
        dhb::ConvertBase(kOctNum, dhb::NumSystem::kOct, dhb::NumSystem::kBin),
        kBinNum);
}

TEST(BaseConvTests, ConvertBaseReturnsOctFromDec) {
    ASSERT_EQ(
        dhb::ConvertBase(kDecNum, dhb::NumSystem::kDec, dhb::NumSystem::kOct),
        kOctNum);
}

TEST(BaseConvTests, ConvertBaseReturnsOctFromHex) {
    ASSERT_EQ(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kHex, dhb::NumSystem::kOct),
        kOctNum);
}

TEST(BaseConvTests, ConvertBaseReturnsOctFromBin) {
    ASSERT_EQ(
        dhb::ConvertBase(kBinNum, dhb::NumSystem::kBin, dhb::NumSystem::kOct),
        kOctNum);
}

TEST(BaseConvTests, ConvertBaseReturnsHexFromDec) {
    ASSERT_EQ(
        dhb::ConvertBase(kDecNum, dhb::NumSystem::kDec, dhb::NumSystem::kHex),
        kHexNum);
}

TEST(BaseConvTests, ConvertBaseReturnsHexFromBin) {
    ASSERT_EQ(
        dhb::ConvertBase(kBinNum, dhb::NumSystem::kBin, dhb::NumSystem::kHex),
        kHexNum);
}

TEST(BaseConvTests, ConvertBaseReturnsHexFromOct) {
    ASSERT_EQ(
        dhb::ConvertBase(kOctNum, dhb::NumSystem::kOct, dhb::NumSystem::kHex),
        kHexNum);
}

TEST(BaseConvTests, ConvertBaseReturnsHexFromHex) {
    ASSERT_EQ(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kHex, dhb::NumSystem::kHex),
        kHexNum);
}

TEST(BaseConvTests, ConvertBaseReturnsBinFromBin) {
    ASSERT_EQ(
        dhb::ConvertBase(kBinNum, dhb::NumSystem::kBin, dhb::NumSystem::kBin),
        kBinNum);
}

TEST(BaseConvTests, ConvertBaseReturnsDecFromDec) {
    ASSERT_EQ(
        dhb::ConvertBase(kDecNum, dhb::NumSystem::kDec, dhb::NumSystem::kDec),
        kDecNum);
}

TEST(BaseConvTests, ConvertBaseReturnsOctFromOct) {
    ASSERT_EQ(
        dhb::ConvertBase(kOctNum, dhb::NumSystem::kOct, dhb::NumSystem::kOct),
        kOctNum);
}

TEST(BaseConvTests, ConvertBaseThrowsInvalidArgumentWhenGivenAnInvalidDecNum) {
    ASSERT_THROW(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kDec, dhb::NumSystem::kHex),
        std::invalid_argument);
}

TEST(BaseConvTests, ConvertBaseThrowsInvalidArgumentWhenGivenAnInvalidBinNum) {
    ASSERT_THROW(
        dhb::ConvertBase(kHexNum, dhb::NumSystem::kBin, dhb::NumSystem::kHex),
        std::invalid_argument);
}

TEST(BaseConvTests, ConvertBaseThrowsInvalidArgumentWhenGivenAnInvalidHexNum) {
    ASSERT_THROW(
        dhb::ConvertBase("BOOM", dhb::NumSystem::kHex, dhb::NumSystem::kDec),
        std::invalid_argument);
}

TEST(BaseConvTests, ConvertBaseThrowsInvalidArgumentWhenGivenAnInvalidOctNum) {
    ASSERT_THROW(
        dhb::ConvertBase("BOOM", dhb::NumSystem::kOct, dhb::NumSystem::kDec),
        std::invalid_argument);
}
