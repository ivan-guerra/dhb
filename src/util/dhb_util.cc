#include "util/dhb_util.hpp"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "base_conversions/base_conversions.hpp"

namespace dhb {

dhb::NumSystem GetNumSystem(const std::string& base) {
    static const std::unordered_map<std::string, dhb::NumSystem>
        kNumSystemLookup = {
            {"bin", dhb::kBin}, {"dec", dhb::kDec}, {"hex", dhb::kHex}};

    if (!kNumSystemLookup.count(base)) {
        throw std::logic_error(base);
    }
    return kNumSystemLookup.at(base);
}

std::string GroupDigits(const std::string& num, int grouping) {
    if ((grouping <= 0) || (grouping >= static_cast<int>(num.size()))) {
        return num;
    }

    std::stack<char> digits;
    for (const char& c : num) {
        digits.push(c);
    }

    std::string group;
    std::vector<std::string> groups;
    while (!digits.empty()) {
        group += digits.top();
        digits.pop();

        if (static_cast<int>(group.size()) == grouping) {
            std::reverse(group.begin(), group.end());
            groups.push_back(group);
            group = "";
        }
    }

    if (!group.empty()) {
        std::reverse(group.begin(), group.end());
        groups.push_back(group);
    }

    std::reverse(groups.begin(), groups.end());
    return std::accumulate(groups.begin(), groups.end(), std::string(),
                           [](const std::string& a, const std::string& b) {
                               return a + (a.empty() ? "" : " ") + b;
                           });
}

std::string SetWidth(const std::string& num, int width) {
    if (width <= 0) {
        return num;
    }

    std::stringstream ss(num);
    ss << std::setfill('0') << std::setw(width) << num;

    return ss.str();
}

std::string StripPrefix(const std::string& num) {
    const std::size_t kCommonPrefixLen = 2;
    const std::string kHexPrefix("0x");
    const std::string kBinPrefix("0b");
    if (num.size() <= kCommonPrefixLen) {
        return num;
    }

    std::string prefix = num.substr(0, kCommonPrefixLen);
    if ((kHexPrefix == prefix) || (kBinPrefix == prefix)) {
        return num.substr(prefix.size());
    }
    return num;
}

}  // namespace dhb
