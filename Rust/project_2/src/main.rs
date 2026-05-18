// project_2: Structs & Enums

// --- Struct ---
struct Rectangle {
    width: f64,
    height: f64,
}

impl Rectangle {
    fn new(width: f64, height: f64) -> Self {
        Self { width, height }
    }

    fn area(&self) -> f64 {
        self.width * self.height
    }

    fn perimeter(&self) -> f64 {
        2.0 * (self.width + self.height)
    }

    fn is_square(&self) -> bool {
        self.width == self.height
    }
}

// --- Enum with data ---
#[derive(Debug, PartialEq)]
enum Shape {
    Circle(f64),           // radius
    Rect(f64, f64),        // width, height
    Triangle(f64, f64),    // base, height
}

impl Shape {
    fn area(&self) -> f64 {
        match self {
            Shape::Circle(r) => std::f64::consts::PI * r * r,
            Shape::Rect(w, h) => w * h,
            Shape::Triangle(b, h) => 0.5 * b * h,
        }
    }
}

fn main() {
    let rect = Rectangle::new(4.0, 6.0);
    println!("area: {}, perimeter: {}, square: {}", rect.area(), rect.perimeter(), rect.is_square());

    let square = Rectangle::new(5.0, 5.0);
    println!("is square: {}", square.is_square());

    let shapes = [
        Shape::Circle(3.0),
        Shape::Rect(4.0, 5.0),
        Shape::Triangle(6.0, 4.0),
    ];
    for shape in &shapes {
        println!("{:?} area = {:.2}", shape, shape.area());
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rectangle_area() {
        let r = Rectangle::new(3.0, 4.0);
        assert_eq!(r.area(), 12.0);
    }

    #[test]
    fn test_rectangle_perimeter() {
        let r = Rectangle::new(3.0, 4.0);
        assert_eq!(r.perimeter(), 14.0);
    }

    #[test]
    fn test_is_square() {
        assert!(Rectangle::new(5.0, 5.0).is_square());
        assert!(!Rectangle::new(3.0, 4.0).is_square());
    }

    #[test]
    fn test_shape_area() {
        assert_eq!(Shape::Rect(4.0, 5.0).area(), 20.0);
        assert_eq!(Shape::Triangle(6.0, 4.0).area(), 12.0);
        let circle_area = Shape::Circle(1.0).area();
        assert!((circle_area - std::f64::consts::PI).abs() < 1e-10);
    }
}
