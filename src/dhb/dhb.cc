#include <getopt.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "base_conversions/base_conversions.hpp"
#include "util/dhb_util.hpp"

static void PrintUsage() {
    std::cout << "usage: dhb [OPTION]... SRC_BASE TGT_BASE NUM" << std::endl;
    std::cout << "convert between numbers in decimal, binary, or hexadecimal"
              << std::endl
              << std::endl;

    std::cout << "REQUIRED ARGUMENTS" << std::endl;
    std::cout
        << "\tSRC_BASE\n\t\tinput number base, one of 'bin', 'dec', or 'hex'"
        << std::endl;
    std::cout
        << "\tTGT_BASE\n\t\toutput number base, one of 'bin', 'dec', or 'hex'"
        << std::endl;

    std::cout << "OPTIONS" << std::endl;
    std::cout << "\t-g,--grouping\n\t\thow to visually group the digits in the "
                 "output number (default\n\t\tbehavior is to concatenate all "
                 "digits)"
              << std::endl;
    std::cout << "\t-w,--width\n\t\tminimum number of digits in the output"
              << std::endl;
    std::cout << "\t-h,--help\n\t\tprint this help message" << std::endl;

    std::cout << "EXAMPLES" << std::endl;
    std::cout << "\tdhb hex dec 0xDEADBEED --> 3735928559" << std::endl;
    std::cout << "\tdhb dec bin 42 --> 101010" << std::endl;
    std::cout << "\tdhb -g 4 dec hex 3735928559 --> DEAD BEEF" << std::endl;
    std::cout << "\tdhb -g 4 -w 12 dec hex 3735928559 --> 0000 DEAD BEEF"
              << std::endl;
}

static void PrintErrAndExit(const std::string& err) {
    std::cerr << "error: " << err << std::endl;
    std::cerr << "try 'dhb --help' for more information" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    struct option long_options[] = {
        {"grouping", required_argument, 0, 'g'},
        {"width", required_argument, 0, 'w'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    /* parse program options */
    int opt = '\0';
    int long_index = 0;
    std::string grouping_str;
    std::string width_str;
    while (-1 != (opt = getopt_long(argc, argv, "hg:w:",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
        switch (opt) {
            case 'g':
                grouping_str = optarg;
                break;
            case 'w':
                width_str = optarg;
                break;
            case 'h':
                PrintUsage();
                return 0;
            case '?':
                return 1;
        }
    }

    /* verify we got all positional arguments */
    if (!argv[optind]) {
        PrintErrAndExit("missing SRC_BASE");
    }
    if (!argv[optind + 1]) {
        PrintErrAndExit("missing TGT_BASE");
    }
    if (!argv[optind + 2]) {
        PrintErrAndExit("missing NUM");
    }

    try {
        std::string src_base_str(argv[optind]);
        std::string tgt_base_str(argv[optind + 1]);
        std::string num(argv[optind + 2]);

        /* identify the source and target bases */
        dhb::NumSystem src_base = dhb::GetNumSystem(src_base_str);
        dhb::NumSystem tgt_base = dhb::GetNumSystem(tgt_base_str);

        /* perform the base conversion */
        num = dhb::StripPrefix(num);
        std::string converted_num = dhb::ConvertBase(num, src_base, tgt_base);

        /* adjust the width of the converted number */
        int width = width_str.empty() ? 0 : std::stoi(width_str);
        converted_num = dhb::SetWidth(converted_num, width);

        /* split converted_num into grouping sized chunks */
        int grouping = grouping_str.empty() ? 0 : std::stoi(grouping_str);
        converted_num = dhb::GroupDigits(converted_num, grouping);

        std::cout << converted_num << std::endl;
    } catch (const std::invalid_argument& e) {
        PrintErrAndExit("invalid number format, check input and arg nums");
    } catch (const std::logic_error& e) {
        PrintErrAndExit("invalid base value '" + std::string(e.what()) + "'");
    } catch (const std::exception& e) {
        PrintErrAndExit(e.what());
    }

    return 0;
}
