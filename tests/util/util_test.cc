#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "base_conversions/base_conversions.hpp"
#include "util/dhb_util.hpp"

TEST(UtilTests, GetNumSystemCanRetrieveNumSystemForValidBase) {
    const std::string kHexBase("hex");
    const std::string kBinBase("bin");
    const std::string kDecBase("dec");
    const std::string kOctBase("oct");

    ASSERT_EQ(dhb::GetNumSystem(kHexBase), dhb::NumSystem::kHex);
    ASSERT_EQ(dhb::GetNumSystem(kBinBase), dhb::NumSystem::kBin);
    ASSERT_EQ(dhb::GetNumSystem(kDecBase), dhb::NumSystem::kDec);
    ASSERT_EQ(dhb::GetNumSystem(kOctBase), dhb::NumSystem::kOct);
}

TEST(UtilTests, GetNumSystemThrowsLogicErrorOnInvalidBase) {
    const std::string kInvalidBaseStr("foo");

    ASSERT_THROW(dhb::GetNumSystem(kInvalidBaseStr), std::logic_error);
}

TEST(UtilTests, GroupDigitsReturnsNumWhenGroupingIsLessThanOrEqualToZero) {
    const std::string kNum("12345");

    ASSERT_EQ(dhb::GroupDigits(kNum, 0), kNum);
    ASSERT_EQ(dhb::GroupDigits(kNum, -1), kNum);
}

TEST(UtilTests, GroupDigitsReturnsNumWhenGroupingIsLargerThanNumLength) {
    const std::string kNum("12345");

    ASSERT_EQ(dhb::GroupDigits(kNum, kNum.size() + 1), kNum);
}

TEST(UtilTests, GroupDigitsReturnsValidEvenAndOddGrouping) {
    const std::string kNum("123456789");
    const std::string kExpectedEvenGrouping("1 23 45 67 89");
    const std::string kExpectedOddGrouping("123 456 789");
    const int kEvenGrouping = 2;
    const int kOddGrouping = 3;

    ASSERT_EQ(dhb::GroupDigits(kNum, kEvenGrouping), kExpectedEvenGrouping);
    ASSERT_EQ(dhb::GroupDigits(kNum, kOddGrouping), kExpectedOddGrouping);
}

TEST(UtilTests, SetWidthReturnsNumWhenGivenWidthLessThanOrEqualToZero) {
    const std::string kNum("12345");

    ASSERT_EQ(dhb::SetWidth(kNum, 0), kNum);
    ASSERT_EQ(dhb::SetWidth(kNum, -1), kNum);
}

TEST(UtilTests, SetWidthReturnsNumWhenGivenWidthLessThanOrEqualToNumLength) {
    const std::string kNum("12345");

    ASSERT_EQ(dhb::SetWidth(kNum, kNum.size()), kNum);     /* equal to */
    ASSERT_EQ(dhb::SetWidth(kNum, kNum.size() - 1), kNum); /* less than */
}

TEST(UtilTests, SetWidthPrependsZeroesToMeetWidthSpec) {
    const std::string kNum("12345");
    const int kTargetWidth = 10;
    const std::string kPaddedNum = dhb::SetWidth(kNum, kTargetWidth);

    ASSERT_EQ(kPaddedNum.size(), kTargetWidth);

    /* check the padded prefix is the right length and is composed of all 0's */
    std::string prefix = kPaddedNum.substr(0, kTargetWidth - kNum.size());
    ASSERT_EQ(prefix.size(), kTargetWidth - kNum.size());
    ASSERT_EQ(prefix, std::string(kTargetWidth - kNum.size(), '0'));

    /* check the original number forms the suffix */
    std::string suffix = kPaddedNum.substr(kTargetWidth - kNum.size());
    ASSERT_EQ(kNum, suffix);
}

TEST(UtilTests, StripPrefixStripsHexPrefix) {
    const std::string kHexNumWithPrefix("0xDEADBEEF");
    const std::string kHexNumNoPrefix("DEADBEEF");

    ASSERT_EQ(dhb::StripPrefix(kHexNumWithPrefix), kHexNumNoPrefix);
}

TEST(UtilTests, StripPrefixStripsBinPrefix) {
    const std::string kBinNumWithPrefix("0b11110000");
    const std::string kBinNumNoPrefix("11110000");

    ASSERT_EQ(dhb::StripPrefix(kBinNumWithPrefix), kBinNumNoPrefix);
}

TEST(UtilTests, StripPrefixStripsOctPrefix) {
    const std::string kOctNumWithPrefix("0o12");
    const std::string kOctNumNoPrefix("12");

    ASSERT_EQ(dhb::StripPrefix(kOctNumWithPrefix), kOctNumNoPrefix);
}

TEST(UtilTests,
     StripPrefixReturnsNumWhenNumLengthIsLessThanOrEqualToPrefixSize) {
    const std::string kNumWithLesserLen("1");
    const std::string kNumWithEqualLen("12");

    ASSERT_EQ(dhb::StripPrefix(kNumWithLesserLen), kNumWithLesserLen);
    ASSERT_EQ(dhb::StripPrefix(kNumWithEqualLen), kNumWithEqualLen);
}

TEST(UtilTests, StripPrefixReturnsNumWhenNumDoesNotIncludePrefix) {
    const std::string kNum("DEADBEEF");

    ASSERT_EQ(dhb::StripPrefix(kNum), kNum);
}
