use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(
        value_enum,
        help = "source number system"
    )]
    src_base: nconv::NumSystem,

    #[arg(
        value_enum,
        help = "target number system"
    )]
    tgt_base: nconv::NumSystem,

    #[arg(help = "a positive integer in the source number system")]
    number: String,

    #[arg(
        short = 'g',
        long,
        default_value_t = 1, 
        value_parser = clap::value_parser!(u32).range(1..=256),
        help = "how to visually group the digits in the output number"
    )]
    grouping: u32,

    #[arg(
        short = 'w',
        long,
        default_value_t = 1, 
        value_parser = clap::value_parser!(u32).range(1..=256),
        help = "minimum number of digits in the output"
    )]
    width: u32,
}

fn main() {
    let args = Args::parse();
    let config = nconv::Config::new(
        args.src_base,
        args.tgt_base,
        args.number,
        args.grouping,
        args.width,
    );

    if let Err(e) = nconv::run(&config) {
        eprintln!("error: {}", e);
        std::process::exit(1);
    }
}
