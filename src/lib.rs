//! A number base conversion library and command-line tool.
//!
//! `nconv` provides functionality to convert numbers between different numeric bases
//! (binary, octal, decimal, and hexadecimal) with additional formatting options.
//!
//! # Features
//!
//! - Convert between binary (base-2), octal (base-8), decimal (base-10), and hexadecimal (base-16)
//! - Support for common number prefixes (0b, 0o, 0x)
//! - Configurable output width with zero padding
//! - Optional digit grouping for improved readability
//!
//! # Examples
//!
//! ```
//! use nconv::{Config, NumSystem};
//!
//! let config = Config {
//!     number: String::from("255"),
//!     src_base: NumSystem::Dec,
//!     tgt_base: NumSystem::Hex,
//!     width: 4,
//!     grouping: 0,
//! };
//!
//! nconv::run(&config).unwrap();  // Prints: 00FF
//! ```
//!
//! # Error Handling
//!
//! The library uses a custom `ConversionError` type to handle various error cases:
//! - Invalid digits for the given base
//! - Number overflow
//! - Invalid base combinations
//! - Mismatched prefixes
use clap::ValueEnum;
use std::fmt::Display;

/// Represents the supported number systems for conversion.
#[derive(Debug, Clone, Copy, PartialEq, ValueEnum)]
pub enum NumSystem {
    /// Binary number system (base 2).
    Bin = 2,
    /// Octal number system (base 8).
    Oct = 8,
    /// Decimal number system (base 10).
    Dec = 10,
    /// Hexadecimal number system (base 16).
    Hex = 16,
}

/// Configuration for number conversion.
pub struct Config {
    /// The source number system to convert from.
    pub src_base: NumSystem,
    /// The target number system to convert to.
    pub tgt_base: NumSystem,
    /// The input number as a string.
    pub number: String,
    /// The size of digit grouping (0 for no grouping).
    pub grouping: u32,
    /// The minimum width for zero-padding the output.
    pub width: u32,
}

impl Config {
    pub fn new(
        src_base: NumSystem,
        tgt_base: NumSystem,
        number: String,
        grouping: u32,
        width: u32,
    ) -> Config {
        Config {
            src_base,
            tgt_base,
            number,
            grouping,
            width,
        }
    }
}

/// Errors that can occur during number system conversion.
#[derive(Debug)]
pub enum ConversionError {
    /// Invalid digit found in input number (contains the invalid character).
    InvalidDigit(char),
    /// Number is too large to be represented (exceeds 128 bit limit).
    NumberOverflow,
    /// Invalid base specified for conversion.
    InvalidBase,
}

impl Display for ConversionError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            ConversionError::InvalidDigit(c) => write!(f, "invalid digit: '{}'", c),
            ConversionError::NumberOverflow => write!(f, "input value exceeds 128 bit limit"),
            ConversionError::InvalidBase => write!(f, "invalid base"),
        }
    }
}

/// Converts a number string from one numeric base to another.
///
/// This function supports conversion between binary, octal, decimal, and hexadecimal number systems.
/// It recognizes common prefixes (0b, 0o, 0x) when they match the source base.
///
/// # Arguments
///
/// * `num` - A string slice containing the number to convert
/// * `src` - The source number system (base) of the input
/// * `target` - The target number system (base) for the output
///
/// # Returns
///
/// * `Ok(String)` - The converted number as a string in the target base
/// * `Err(ConversionError)` - If the conversion fails due to:
///   - Invalid digits for the source base
///   - Number overflow
///   - Invalid base combination
///   - Mismatched prefix and source base
///
/// # Examples
///
/// ```
/// use nconv::{convert_base, NumSystem};
///
/// let result = convert_base("1010", NumSystem::Bin, NumSystem::Dec);
/// assert_eq!(result.unwrap(), "10");
///
/// let hex_result = convert_base("0xFF", NumSystem::Hex, NumSystem::Dec);
/// assert_eq!(hex_result.unwrap(), "255");
/// ```
pub fn convert_base(
    num: &str,
    src: NumSystem,
    target: NumSystem,
) -> Result<String, ConversionError> {
    let digits = "0123456789ABCDEF";

    // Handle prefixes and determine actual number string
    let (num_str, inferred_src) = match num.to_lowercase().as_str() {
        s if s.starts_with("0x") && src == NumSystem::Hex => (&num[2..], Some(NumSystem::Hex)),
        s if s.starts_with("0o") && src == NumSystem::Oct => (&num[2..], Some(NumSystem::Oct)),
        s if s.starts_with("0b") && src == NumSystem::Bin => (&num[2..], Some(NumSystem::Bin)),
        s if s.starts_with("0x") || s.starts_with("0o") || s.starts_with("0b") => {
            return Err(ConversionError::InvalidBase)
        }
        _ => (num, None),
    };

    // Use inferred source base if available, otherwise use provided src
    let source_base = inferred_src.unwrap_or(src);

    // First convert to decimal
    let mut decimal = 0u128;
    for c in num_str.chars() {
        let digit = if c.is_ascii_digit() {
            let d = c as u128 - '0' as u128;
            if d >= source_base as u128 {
                return Err(ConversionError::InvalidDigit(c));
            }
            d
        } else {
            let d = (c.to_ascii_uppercase() as u128) - ('A' as u128) + 10;
            if d >= source_base as u128 {
                return Err(ConversionError::InvalidDigit(c));
            }
            d
        };

        decimal = decimal
            .checked_mul(source_base as u128)
            .ok_or(ConversionError::NumberOverflow)?;
        decimal = decimal
            .checked_add(digit)
            .ok_or(ConversionError::NumberOverflow)?;
    }

    // Then convert to target base
    if decimal == 0 {
        return Ok("0".to_string());
    }

    let mut result = Vec::new();
    let mut num = decimal;
    while num > 0 {
        let remainder = (num % target as u128) as usize;
        result.push(
            digits
                .chars()
                .nth(remainder)
                .ok_or(ConversionError::InvalidBase)?,
        );
        num /= target as u128;
    }

    Ok(result.iter().rev().collect())
}

/// Groups digits in a number string with specified spacing.
///
/// # Arguments
/// * `num` - The number string to group.
/// * `grouping` - Number of digits per group (0 for no grouping).
///
/// # Returns
/// A new string with digits grouped using spaces.
pub fn group_digits(num: &str, grouping: u32) -> String {
    if grouping == 0 {
        return num.to_string();
    }

    let mut result = num.chars().rev().collect::<String>();
    let mut i = grouping as usize;
    while i < result.len() {
        result.insert(i, ' ');
        i += grouping as usize + 1;
    }
    result.chars().rev().collect()
}

/// Pads a number string with leading zeros to reach specified width.
///
/// # Arguments
/// * `num` - The number string to pad.
/// * `width` - The minimum width to pad to.
///
/// # Returns
/// A new string padded with leading zeros if needed.
pub fn pad_width(num: &str, width: u32) -> String {
    if num.len() < width as usize {
        let padding = "0".repeat(width as usize - num.len());
        format!("{}{}", padding, num)
    } else {
        num.to_string()
    }
}

/// Executes the number conversion process based on the provided configuration.
///
/// This function performs the following steps:
/// 1. Converts the number from source base to target base.
/// 2. Pads the result to the specified width.
/// 3. Groups digits according to the grouping configuration.
/// 4. Prints the final result to stdout.
///
/// # Arguments
///
/// * `config` - Reference to a Config struct containing conversion parameters.
///
/// # Returns
///
/// * `Ok(())` - If the conversion and output were successful.
/// * `Err(ConversionError)` - If any step in the conversion process fails.
///
/// # Examples
///
/// ```
/// use nconv::{Config, NumSystem};
///
/// let config = Config {
///     number: String::from("1010"),
///     src_base: NumSystem::Bin,
///     tgt_base: NumSystem::Dec,
///     width: 0,
///     grouping: 0,
/// };
///
/// nconv::run(&config).unwrap();  // Prints: 10
/// ```
pub fn run(config: &Config) -> Result<(), ConversionError> {
    let result = convert_base(&config.number, config.src_base, config.tgt_base)?;
    let result = pad_width(&result, config.width);
    let result = group_digits(&result, config.grouping);
    println!("{}", result);

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn convert_base_handles_valid_prefixes_correctly() -> Result<(), ConversionError> {
        assert_eq!(convert_base("0xFF", NumSystem::Hex, NumSystem::Dec)?, "255");
        assert_eq!(
            convert_base("0b1010", NumSystem::Bin, NumSystem::Dec)?,
            "10"
        );
        assert_eq!(convert_base("0o77", NumSystem::Oct, NumSystem::Dec)?, "63");
        Ok(())
    }

    #[test]
    fn convert_base_returns_invalid_base_error_when_given_incorrect_src_prefix() {
        // Test hex prefix with binary base
        match convert_base("0xFF", NumSystem::Bin, NumSystem::Dec) {
            Err(ConversionError::InvalidBase) => (),
            _ => panic!("Expected InvalidBase error for hex prefix with binary base"),
        }

        // Test binary prefix with hex base
        match convert_base("0b10", NumSystem::Hex, NumSystem::Dec) {
            Err(ConversionError::InvalidBase) => (),
            _ => panic!("Expected InvalidBase error for binary prefix with hex base"),
        }

        // Test octal prefix with binary base
        match convert_base("0o77", NumSystem::Bin, NumSystem::Dec) {
            Err(ConversionError::InvalidBase) => (),
            _ => panic!("Expected InvalidBase error for octal prefix with binary base"),
        }
    }

    #[test]
    fn convert_base_successfully_converts_when_prefix_is_excluded() -> Result<(), ConversionError> {
        // Non-prefixed numbers should work
        assert_eq!(convert_base("FF", NumSystem::Hex, NumSystem::Dec)?, "255");
        assert_eq!(convert_base("1010", NumSystem::Bin, NumSystem::Dec)?, "10");
        assert_eq!(convert_base("77", NumSystem::Oct, NumSystem::Dec)?, "63");
        Ok(())
    }

    #[test]
    fn convert_base_returns_invalid_digit_error_on_invalid_digits() {
        assert!(matches!(
            convert_base("0xGG", NumSystem::Hex, NumSystem::Dec),
            Err(ConversionError::InvalidDigit(_))
        ));
        assert!(matches!(
            convert_base("0b12", NumSystem::Bin, NumSystem::Dec),
            Err(ConversionError::InvalidDigit(_))
        ));
        assert!(matches!(
            convert_base("0o89", NumSystem::Oct, NumSystem::Dec),
            Err(ConversionError::InvalidDigit(_))
        ));
    }

    #[test]
    fn convert_base_returns_number_overflow_when_given_greater_than_128_bit_number() {
        // Test with even larger numbers that will definitely overflow
        let large_hex = format!("0x{}", "F".repeat(256)); // Much larger hex number
        let large_bin = format!("0b{}", "1".repeat(1024)); // Much larger binary number
        assert!(matches!(
            convert_base(&large_hex, NumSystem::Hex, NumSystem::Dec),
            Err(ConversionError::NumberOverflow)
        ));
        assert!(matches!(
            convert_base(&large_bin, NumSystem::Bin, NumSystem::Dec),
            Err(ConversionError::NumberOverflow)
        ));
    }

    #[test]
    fn convert_base_successfull_converts_0_to_all_bases() -> Result<(), ConversionError> {
        assert_eq!(convert_base("0x0", NumSystem::Hex, NumSystem::Dec)?, "0");
        assert_eq!(convert_base("0b0", NumSystem::Bin, NumSystem::Dec)?, "0");
        assert_eq!(convert_base("0o0", NumSystem::Oct, NumSystem::Dec)?, "0");
        Ok(())
    }

    #[test]
    fn pad_width_successfully_applies_padding() {
        assert_eq!(pad_width("42", 4), "0042");
        assert_eq!(pad_width("123", 5), "00123");
        assert_eq!(pad_width("7", 3), "007");
    }

    #[test]
    fn pad_width_does_not_apply_padding_when_input_is_wide_enough() {
        assert_eq!(pad_width("123", 3), "123");
        assert_eq!(pad_width("456", 2), "456");
        assert_eq!(pad_width("789", 1), "789");
    }

    #[test]
    fn pad_width_successfully_pads_empty_str() {
        assert_eq!(pad_width("", 3), "000");
        assert_eq!(pad_width("", 1), "0");
        assert_eq!(pad_width("0", 2), "00");
    }

    #[test]
    fn pad_width_ignores_request_for_zero_padding() {
        assert_eq!(pad_width("123", 0), "123");
    }

    #[test]
    fn pad_width_successuly_pads_large_numbers() {
        assert_eq!(
            pad_width("12345678901234567890", 25),
            "0000012345678901234567890"
        );
        assert_eq!(
            pad_width("12345678901234567890", 20),
            "12345678901234567890"
        );
    }

    #[test]
    fn group_digits_successfully_groups_digits() {
        assert_eq!(group_digits("1234", 3), "1 234");
        assert_eq!(group_digits("123456", 3), "123 456");
        assert_eq!(group_digits("1234567", 3), "1 234 567");
    }

    #[test]
    fn group_digits_can_handle_too_few_digits() {
        assert_eq!(group_digits("", 3), "");
        assert_eq!(group_digits("1", 3), "1");
        assert_eq!(group_digits("12", 3), "12");
        assert_eq!(group_digits("123", 3), "123");
    }

    #[test]
    fn group_digits_successfully_groups_digits_of_diff_sizes() {
        assert_eq!(group_digits("1234", 2), "12 34");
        assert_eq!(group_digits("123456", 2), "12 34 56");
        assert_eq!(group_digits("12345", 4), "1 2345");
    }

    #[test]
    fn group_digits_makes_exact_groupings() {
        assert_eq!(group_digits("123123", 3), "123 123");
        assert_eq!(group_digits("12341234", 4), "1234 1234");
    }

    #[test]
    fn group_digits_groups_large_numbers_successfully() {
        assert_eq!(
            group_digits("1234567890123456789", 3),
            "1 234 567 890 123 456 789"
        );
        assert_eq!(
            group_digits("12345678901234567890", 4),
            "1234 5678 9012 3456 7890"
        );
    }
}
