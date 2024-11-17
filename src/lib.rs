use clap::ValueEnum;
use std::fmt::Display;

#[derive(Debug, Clone, Copy, ValueEnum)]
pub enum NumSystem {
    Bin = 2,
    Oct = 8,
    Dec = 10,
    Hex = 16,
}

pub struct Config {
    pub src_base: NumSystem,
    pub tgt_base: NumSystem,
    pub number: String,
    pub grouping: u32,
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

#[derive(Debug)]
pub enum ConversionError {
    InvalidDigit(char),
    NumberOverflow,
    InvalidBase,
}

impl Display for ConversionError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            ConversionError::InvalidDigit(c) => write!(f, "invalid digit: '{}'", c),
            ConversionError::NumberOverflow => write!(f, "number overflow"),
            ConversionError::InvalidBase => write!(f, "invalid base"),
        }
    }
}

pub fn convert_base(
    num: &str,
    src: NumSystem,
    target: NumSystem,
) -> Result<String, ConversionError> {
    let digits = "0123456789ABCDEF";

    // Handle prefixes and determine actual number string
    let (num_str, inferred_src) = match num.to_lowercase().as_str() {
        s if s.starts_with("0x") => (&num[2..], Some(NumSystem::Hex)),
        s if s.starts_with("0o") => (&num[2..], Some(NumSystem::Oct)),
        s if s.starts_with("0b") => (&num[2..], Some(NumSystem::Bin)),
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

pub fn group_digits(num: &str, grouping: u32) -> String {
    let mut result = num.chars().rev().collect::<String>();
    let mut i = grouping as usize;
    while i < result.len() {
        result.insert(i, '_');
        i += grouping as usize + 1;
    }
    result.chars().rev().collect()
}

pub fn pad_width(num: &str, width: u32) -> String {
    if num.len() < width as usize {
        let padding = "0".repeat(width as usize - num.len());
        format!("{}{}", padding, num)
    } else {
        num.to_string()
    }
}

pub fn run(config: &Config) -> Result<(), ConversionError> {
    let result = convert_base(&config.number, config.src_base, config.tgt_base)?;
    let result = group_digits(&result, config.grouping);
    let result = pad_width(&result, config.width);
    println!("{}", result);

    Ok(())
}
