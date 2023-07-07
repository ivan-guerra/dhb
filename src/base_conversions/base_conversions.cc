#include "base_conversions/base_conversions.hpp"

#include <gmpxx.h>

#include <algorithm>
#include <string>

namespace dhb {

std::string ConvertBase(const std::string& num, const NumSystem src,
                        const NumSystem target) {
    const mpz_class kTargetBase(static_cast<int>(target));
    const std::string kDigits("0123456789ABCDEF");

    std::string converted_num;
    mpz_class num_mp(num, static_cast<int>(src));
    while (num_mp) {
        mpz_class idx = num_mp % kTargetBase;
        converted_num += kDigits[idx.get_si()];
        num_mp /= kTargetBase;
    }
    std::reverse(converted_num.begin(), converted_num.end());

    return converted_num;
}

}  // namespace dhb
