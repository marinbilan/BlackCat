#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

using namespace std;


// ---- EXAMPLE 1 ----

// Color reprasents the dependency on the OpenGl
// graphics library
struct Color
{
public:
    float red;
    float green;
    float blue;
   
    // float getRed() { return red; }
};
// std::string to_string(Color color); // to implement

/*
Note that no base class
No virtual functions
No draw functions
*/
class Circle
{
public:
    explicit Circle(double rad) : radius(rad)
    {
        std::cout << "Constructor Circle is called! (Free instance)" << '\n';
    }
   
    double getRadius() const noexcept
    {
        return radius;
    };
   
private:
double radius;
};


/*
Circles and Squares
... do not need base class
... do not know about each other
... no knowladge about operations
... do not depend on impl details

*/

class Square
{
public:
    explicit Square(double s) : side(s)
    {
        std::cout << "Constructor Square is called! (Free instance)" << '\n';
    }
   
    double getSide() const noexcept
    {
        return side;
    };
   
private:
double side;
};



// ----


// Wrap ShapeConcept and ShapeModel in Shape class
class Shape
{
private:

// This type of pattern is called - External Polymorphism Desing Pattern (IMPORTANT)
struct ShapeConcept
{
    virtual ~ShapeConcept() {}
   
    // We introduce things we want to do on any kind of shape
    virtual void serialize() const = 0;
    virtual void draw() const = 0;
    virtual std::unique_ptr<ShapeConcept> clone() const = 0; // The Prototype Design Pattern
};


// Class that combines Shape and Strategy on that shape
// Shape model is wrapper for real geometry (circle ...)
template<typename GeomShape, typename DrawStrategy>
struct ShapeModel : ShapeConcept
{
    ShapeModel(GeomShape const& shape, DrawStrategy strategy) : shape_(shape), strategy_(strategy)
    {
        std::cout << "ShapeModel (pimpl) constructor is called!" << '\n';
    }

    virtual std::unique_ptr<ShapeConcept> clone() const override
    {
        return std::make_unique<ShapeModel>(*this); // The Prototype Design Pattern
    }
   
    void serialize() const override
    {
        // Specific implementation free functions (see below)
        // serialize(shape_, ...);
    }

    void draw() const override
    {
        std::cout << "[4] draw() - from shape model" << '\n';
        // Specific implementation - Here goes strategy!
        // draw(shape_, ...);
        Color red;
        // Call strategy
        strategy_(shape_, red);
    }
   
GeomShape shape_;
DrawStrategy strategy_;
};


// Shape is wrapper class
/*
Despite being defined inside the class definition, these friend functions are
free functions and injected into the surrounding namespace.
*/
friend void draw(Shape const& shape)
{
    // Outter class (Shape) - Inner class (object) ()
    std::cout << "[3] friend draw() - shape.pimpl->draw()" << '\n';
    shape.pimpl->draw();
}

std::unique_ptr<ShapeConcept> pimpl;

public:
// This constructor creates Bridge (design pattern) - Now we have erased type
    template<typename GeomShape, typename DrawStrategy>
    Shape(GeomShape const& shape, DrawStrategy strategy) : pimpl(new ShapeModel<GeomShape,DrawStrategy>(shape, strategy))
    {
        std::cout << "Constructor Shape is called and pimpl (ShapeModel) is created with strategy!" << '\n';

    }
};


// External free methods (requested by pattern)
// External dependencies
// Those methods can be in h files or we can link them
// or also can be linked in runtime
// This can be in many files - concrete implementation (OpenGL, Vulcan, ...)
void serialize(Circle const&) {}
void draw(Circle const&, Color const&) { std::cout << "[5] Circle draw() - low level implementation" << '\n'; }

void serialize(Square const&) {}
void draw(Square const&, Color const&) { std::cout << "[5] Square draw() - low level implementation" << '\n'; }


// **** DRAW ALL SHAPES ****
void drawAllShapes(std::vector<Shape> const& shapes)
{
    for(auto const& shape : shapes)
    {
        // This is friend function. Function outside can be used outside of class
        // but can use members from class
        draw(shape);
    }
}


// ---- EXAMPLE 2 ----

struct Plus1
{
    int call(int x) const
    {
        return x+1;
    }
};

// In complete implementation this is wrapped inside outer class
struct AbstractCallback
{
    virtual int call(int) const = 0;
};

// Pass concrete instance of Plus1 with inner strategy (call)
template<class T>
struct WrappingCallback : AbstractCallback
{
    const T *cb_;
   
    explicit WrappingCallback(const T &cb) : cb_(&cb)
    {}
   
    int call(int x) const override
    {
        // Call external method via strategy pattern
        return cb_->call(x);
       
    }
};

// AbstractCallback is interface
// This is outside draw method
int run_twice(const AbstractCallback& callback)
{
    return callback.call(1) + callback.call(1);
}


// ---- EXAMPLE 3 ----

// Library
template <typename T>
void draw(const T& x, ostream& out, size_t position)
{
    std::cout << "4. draw - outer" << '\n';
    out << string(position, ' ') << x << endl;
}

class object_t
{
public:
    template <typename T>
    object_t(T x) : self_(new model<T>(move(x)))
    {
        std::cout << "Constructor of outer shared_ptr<const concept_t> self_ object" << '\n';
    }

//    object_t(const object_t& x) : self_(x.self_->copy_())
//    { }
//
//    object_t(object_t&&) noexcept = default;
//
//    object_t& operator=(const object_t& x)
//    {
//        object_t tmp(x);
//        *this = move(tmp);
//        return *this;
//    }
//
//    object_t& operator=(object_t&&) noexcept = default;

    friend void draw(const object_t& x, ostream& out, size_t position)
    {
        std::cout << "2. draw - friend" << '\n';
        x.self_->draw_(out, position);
    }

private:
    struct concept_t
    {
        virtual ~concept_t() = default;
//        virtual concept_t* copy_() const = 0;
        virtual void draw_(ostream&, size_t) const = 0;
    };

    template <typename T>
    struct model : concept_t
    {
        model(T x) : data_(move(x))
        {
            std::cout << "Constructor of inner T data_ object" << '\n';    
        }

//        concept_t* copy_() const
//        {
//            return new model(*this);
//        }

        void draw_(ostream& out, size_t position) const
        {
            std::cout << "3. draw - inner - CALL lib" << '\n';
            draw(data_, out, position);
        }

        T data_;
    };

    shared_ptr<const concept_t> self_;
//    unique_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;


// ********
// This is independent method called (first) in main method
void draw(const document_t& x, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    std::cout << "1. draw - " << '\n';
    // STILL NOT CALLED draw method inside object - called friend method
    for (auto& e : x) draw(e, out, position + 2);
    out << string(position, ' ') << "</document>" << endl;
}
// ********

using history_t = vector<document_t>;

void commit(history_t& x)
{
    assert(x.size());
    x.push_back(x.back());
}

void undo(history_t& x)
{
    assert(x.size());
    x.pop_back();
}

document_t& current(history_t& x)
{
    std::cout << "Get last vector (document_t) in vector h (history_t) - history_t& x.back()" << '\n';
    assert(x.size());
    return x.back();
}

/******************************************************************************/
// Client

class my_class_t
{
    /* ... */
};

/*
void draw(const my_class_t&, ostream& out, size_t position)
{
    std::cout << " xxxx MB " << '\n';
    out << string(position, ' ') << "xxxx my_class_t" << endl;
}
*/

template <typename T>
void drawWidget(const T& x, ostream& out, size_t position)
{
    std::cout << "Widget draw" << '\n';
    out << string(position, ' ') << x << endl;
}


class Widget
{
public:
    Widget(const int id) : m_id(id) {}
   
    friend ostream& operator<<(ostream& os, const Widget& w);
   
private:
int m_id;
};


ostream& operator<<(ostream& os, const Widget& w)
{
        os << " My class test" << '\n';
        return os;
}


#if 0
g++ -std=c++11 -o value-semantics-unique.exe value-semantics-unique.cpp && value-semantics-unique.exe

<document>
  0
  Hello!
</document>
--------------------------
<document>
  0
  World
  <document>
    0
    Hello!
  </document>
  my_class_t
</document>
--------------------------
<document>
  0
  Hello!
</document>

#endif