#ifndef DHB_UTIL_HPP_
#define DHB_UTIL_HPP_

#include <string>

#include "base_conversions/base_conversions.hpp"

namespace dhb {

/**
 * Convert \p base to a dhb::NumSystem type.
 *
 * @param [in] base A string label for one of the supported bases: 'bin', 'dec',
 *                  or 'hex'.
 *
 * @throws std::logic_error When \p is not one of 'bin', 'dec', or 'hex'.
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
 *          single space.
 */
std::string GroupDigits(const std::string& num, int grouping);

}  // namespace dhb

#endif
