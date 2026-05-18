// project_1: Ownership & Borrowing

// Takes ownership — caller can't use s after this
fn take_ownership(s: String) -> usize {
    s.len()  // consumes s, returns length
}

// Borrows immutably — caller keeps ownership
fn borrow(s: &String) -> usize {
    s.len()
}

// Borrows mutably — caller keeps ownership, value may change
fn append_exclamation(s: &mut String) {
    s.push('!');
}

// Slices — reference to part of a collection
fn first_word(s: &str) -> &str {
    let bytes = s.as_bytes();
    for (i, &byte) in bytes.iter().enumerate() {
        if byte == b' ' {
            return &s[..i];
        }
    }
    s
}

fn main() {
    // Ownership move
    let s1 = String::from("hello");
    let len = take_ownership(s1); // s1 moved, can't use it after
    println!("length was {len}");

    // Borrowing
    let s2 = String::from("world");
    println!("borrowed length: {}", borrow(&s2));
    println!("s2 still valid: {s2}");

    // Mutable borrow
    let mut s3 = String::from("hello");
    append_exclamation(&mut s3);
    println!("after mutation: {s3}");

    // Slices
    let sentence = String::from("hello world");
    println!("first word: {}", first_word(&sentence));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_borrow_does_not_consume() {
        let s = String::from("rust");
        assert_eq!(borrow(&s), 4);
        assert_eq!(borrow(&s), 4); // still usable
    }

    #[test]
    fn test_append_exclamation() {
        let mut s = String::from("hi");
        append_exclamation(&mut s);
        assert_eq!(s, "hi!");
    }

    #[test]
    fn test_first_word() {
        assert_eq!(first_word("hello world"), "hello");
        assert_eq!(first_word("rust"), "rust");
        assert_eq!(first_word("one two three"), "one");
    }
}
