#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "DesignPatterns.h"


// TEST FIXTURE //
class DesignPatternsTest : public ::testing::Test
{
public:

	DesignPatternsTest()
	{
	}

	~DesignPatternsTest()
	{
	}
};



/*
Problems in SW development
1] Change
2] Dependency - real problem

  [Shape]
|         |
[Circle]  [Square]
If we want to render - we should inherit next layer
|       |
[OpenGl]  [Vulkan]

How would we handle serialization?
Next layer of inheritance for each lib ...

Many derived classes ... NOT GOOD
Bad class names

Deep inheritance structure
Code duplication
----

Classic Solution:
From Gang of Four Book:
Strategy Pattern
----------------

Shape  // High Level
|
Circle  // Architecture Boundary
// Broken dependency
draw()  <>----> DrawStrategy (virtual draw() = 0)
                impl in derived classes  // Low level (change all the time)

For example std::accumulate is realized as Strategy Pattern
// std::plus<>{} is compile time strategy. Dependency injection
std::accumulate(begin(numVec), end(numVec), 0, std::plus<>{});

std::vector
template<
    class T,
    class Allocator = std::allocator<T>  // strategy
> class vector;

// Unordered set has 3 strategies (hash, equal and allocator)

std::unique_ptr - takes deleter (that is strategy)
template<
    class T,
    class Deleter = std::default_delete<T>  // strategy
> class unique_ptr;
   

----
TYPE ERASURE is not:
void pointer (void*)
pointer to base
std::variant

TYPE ERASURE is:
Template Constructor
Non virtual interface
Mix of three design patterns:
    External Polimorphism
    Bridge
    Prototype
*/

TEST_F(DesignPatternsTest, Test_DP_TypeErasure_Ex1)
{
    // The External Polymorphism Design Pattern
    // 1st part of The External Polymorphism Design Pattern development
    /*
    // using Shapes = std::vector<std::unique_ptr<ShapeConcept>>;
   
    // Not red but generic (TODO: Change this)
    auto drawShape = [] (auto const& shape, auto const& color) { draw(shape, color); };
   
    using DrawStrategy = decltype(drawShape);
   
    // Create some derived Shapes
    std::vector<std::unique_ptr<ShapeConcept>> shapes;

    shapes.emplace_back(std::make_unique<ShapeModel<Circle, DrawStrategy>>( Circle{2.0}, drawShape ) );
    shapes.emplace_back(std::make_unique<ShapeModel<Square, DrawStrategy>>( Square{2.0}, drawShape ) );
   
    // Drawing all shapes via base ShapeConcept (base class) pointer
    */
   
    // 2nd part of The External Polymorphism Design Pattern development
    // We create lambda with method we want to invoke

    std::cout << "[1] main Create (lambda) draw() strategy using (wrapping) free draw() function" << '\n';
    auto drawShape = [] (auto const& shape, auto const& color) { draw(shape, color); };
   
    // Creating some shapes
    std::vector<Shape> shapes;
    std::cout << "[2] main Create Shape (Circle, Square ...)" << '\n';
    shapes.emplace_back(Circle(2.0), drawShape);
    // shapes.emplace_back( Square(1.5), drawShape);
    // shapes.emplace_back( Circle(4.2), drawShape);

    // Drawing all shapes
    std::cout << "[3] main Draw all shapes" << '\n';
    drawAllShapes( shapes );
}


TEST_F(DesignPatternsTest, Test_DP_TypeErasure_Ex2)
{
cout <<"---- START ----" << '\n';
   
    printf("%d\n", run_twice( WrappingCallback<Plus1>(Plus1{}) ));
}


TEST_F(DesignPatternsTest, Test_DP_TypeErasure_Ex3)
{
    Widget w(0);
    object_t tempObj(w);
    draw(w, cout, 0);
   
    // drawWidget(w, cout, 0);
   
    // using history_t = vector<document_t>;
    // using document_t = vector<object_t>;
/*
    history_t h(1);
    current(h).emplace_back(0);
    current(h).emplace_back(string("Hello!"));

    Widget w(0);
    current(h).emplace_back(w);

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    commit(h);

    current(h).emplace_back(current(h));
    current(h).emplace_back(my_class_t());
    current(h)[1] = string("World");

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    undo(h);

    draw(current(h), cout, 0);
    */
}