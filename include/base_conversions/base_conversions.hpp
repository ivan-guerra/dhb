#ifndef BASE_CONVERSIONS_HPP_
#define BASE_CONVERSIONS_HPP_

#include <string>

namespace dhb {

/** @brief Supported number systems. */
enum NumSystem : int {
    kDec = 10, /**< Decimal or base 10. */
    kHex = 16, /**< Hexadecimal or base 16. */
    kBin = 2,  /**< Binary or base 2. */
    kOct = 8,  /**< Octal or base 8. */
};

/**
 * Convert the input \p num string from the source to target number system.
 *
 * @param [in] num A string in the \p src number system notation.
 * @param [in] src The source NumSystem
 * @param [in] target The target NumSystem.
 *
 * @throws std::invalid_argument Triggered by an invalid \p num input (e.g., an
 *                               empty string or number with invalid format).
 *
 * @returns The string representation of \p num in the \p target number system.
 */
std::string ConvertBase(const std::string& num, const NumSystem src,
                        const NumSystem target);

}  // namespace dhb

#endif
