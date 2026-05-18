// project_3: Traits & Generics
use std::env;
use std::fs;
use std::process;
use std::error::Error;


fn main() {
    let args: Vec<String> = env::args().collect();

    // dbg!(args);
    
    // Moved to method
    // let query = &args[1];
    // let file_path = &args[2];

    // let (query, file_path) = parse_confing(&args);
    // let config = parse_confing(&args);
    // let config = Config::new(&args);
    let config = Config::build(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {err}");
        process::exit(1);
    });

    println!("Searching for {}", config.query);
    println!("In file {}", config.file_path);

    if let Err(e) = run(config) {
        println!("Application error: {e}");
        process::exit(1);
    }
    // let contents = fs::read_to_string(config.file_path).expect("Should have been able to read the file");
    // println!("With text:\n{contents}");
    /*

    */
}


fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.file_path)?;

    println!("With text:\n{contents}");

    Ok(())
}


struct Config {
    query: String,
    file_path: String,
}


/*
impl Config {
    fn new(args: &[String]) -> Config {
        if args.len() < 3 {
            panic!("not enough arguments");
        }

        let query = args[1].clone();
        let file_path = args[2].clone();

        Config { query, file_path }
    }
}
*/
impl Config {
    fn build(args: &[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("not enough arguments");
        }

        let query = args[1].clone();
        let file_path = args[2].clone();

        Ok(Config { query, file_path })
    }
}


/*
fn parse_confing(args: &[String]) -> (&str, &str) {
    let query = &args[1];
    let file_path = &args[2];

    (query, file_path)
}
*/

fn parse_confing(args: &[String]) -> Config {
    let query = args[1].clone();
    let file_path = args[2].clone();

    Config { query, file_path }
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn command_line_test() {

    }

    /*
    #[test]
    fn test_article_summarize() {
        let a = Article {
            title: String::from("Title"),
            author: String::from("Author"),
            content: String::from("Content here."),
        };
        assert!(a.summarize().contains("Title"));
        assert!(a.summarize().contains("Author"));
    }

    #[test]
    fn test_tweet_summarize() {
        let t = Tweet {
            username: String::from("user"),
            text: String::from("hello world"),
        };
        assert_eq!(t.summarize(), "user: hello world");
    }

    #[test]
    fn test_largest() {
        assert_eq!(*largest(&[3, 1, 4, 1, 5, 9, 2, 6]), 9);
    }

    #[test]
    fn test_pair_larger() {
        let p = Pair::new(7, 3);
        assert_eq!(*p.larger(), 7);
    }
    */
}
