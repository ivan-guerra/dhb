#ifndef DHB_UTIL_HPP_
#define DHB_UTIL_HPP_

#include <string>

#include "base_conversions/base_conversions.hpp"

namespace dhb {

/**
 * Convert \p base to a dhb::NumSystem type.
 *
 * @param [in] base A string label for one of the supported bases: 'bin', 'dec',
 *                  'oct', or 'hex'.
 *
 * @throws std::logic_error When \p base is not one of 'bin', 'dec', 'oct', or
 *                          'hex'.
 *
 * @returns The dhb::NumSystem equivalent of \p base.
 */
dhb::NumSystem GetNumSystem(const std::string& base);

/**
 * Group digits in \p num in segments of size \p grouping.
 *
 * @param [in] num A string of digits.
 * @param [in] grouping How many digits are allocated per segment.
 *
 * @returns The digits of \p num grouped into \p grouping segments seperated by
 *          a single space. If given a \p grouping value larger than \p num
 *          length or less than or equal to 0, GroupDigits() returns \p num.
 */
std::string GroupDigits(const std::string& num, int grouping);

/**
 * Pad \p num with zeroes on the left until the target \p width is met.
 *
 * @param [in] num Any binary, decimal, octal, or hexadecimal number (excluding
 *                 prefixes).
 * @param [in] width Minimum number of characters in the output number string.
 *
 * @returns \p num padded with zeroes on the left until the target \p width is
 *          met. If \p width is less than the current \p num length or less
 *          than or equal to zero, \p num is returned.
 */
std::string SetWidth(const std::string& num, int width);

/**
 * Strip a hexadecimal (0x), binary (0b), or octal (0o) prefix.
 *
 * @param [in] num A string of digits.
 *
 * @returns A new string copy of \p num with any prefixes removed. If \p num
 *          did not have a prefix, StripPrefix() returns \p num.
 */
std::string StripPrefix(const std::string& num);

}  // namespace dhb

#endif
