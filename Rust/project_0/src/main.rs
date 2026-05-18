// project_0: Basics — variables, types, functions

fn add(a: i32, b: i32) -> i32 {
    a + b
}

fn is_even(n: i32) -> bool {
    n % 2 == 0
}

fn celsius_to_fahrenheit(c: f64) -> f64 {
    c * 9.0 / 5.0 + 32.0
}

fn main() {
    // Variables (immutable by default, mutable with mut)
    let x = 5;
    let mut y = 10;
    y += 1;
    println!("x = {x}, y = {y}");

    // Basic types
    let flag: bool = true;
    let pi: f64 = 3.14159;
    let letter: char = 'R';
    println!("flag={flag}, pi={pi}, letter={letter}");

    // Functions
    println!("add(3, 4) = {}", add(3, 4));
    println!("is_even(7) = {}", is_even(7));
    println!("20°C = {}°F", celsius_to_fahrenheit(20.0));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
        assert_eq!(add(-1, 1), 0);
    }

    #[test]
    fn test_is_even() {
        assert!(is_even(4));
        assert!(!is_even(7));
    }

    #[test]
    fn test_celsius_to_fahrenheit() {
        assert_eq!(celsius_to_fahrenheit(0.0), 32.0);
        assert_eq!(celsius_to_fahrenheit(100.0), 212.0);
    }
}
