


class Element {
public:
    Element(int id) : m_id(id) {}

    int getId() const {
        return m_id;
    }

    bool operator==(const Element& el) {
        if(m_id == el.getId()) {
            return true;
        }

        return false;
    }

    bool operator<(const Element& el) {
        if(m_id < el.getId()) {
            return true;
        }

        return false;
    }

private:
int m_id;
};


// Helper method
bool isBiggerMethod(const Element& lhs, const Element& rhs) {
    std::cout << "... sorting ... lhs: " << lhs.getId() << " rhs: " << rhs.getId() << '\n';
    return lhs.getId() < rhs.getId();
}

// Helper functor
class IsBiggerFunctor {
public:
    bool operator()(const Element& lhs, const Element& rhs) {
        return lhs.getId() < rhs.getId();
    }
};

void testF() {

    std::cout << "__Algos Containers" << '\n';
}