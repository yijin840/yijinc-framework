#include <iostream>
using namespace std;

class DeleteObject {
public:
    explicit DeleteObject(const int n) :
        n_(n) {}

    DeleteObject(const DeleteObject& n) :
        n_(n.n_) {}

    [[nodiscard]] int getN_() const { return n_; }

    void operator()(const int* p) const {
        std::cout << "delete object: " << n_ << std::endl;
        delete p;
    }

private:
    int n_;
};

std::ostream& operator<<(std::ostream& lhs, const DeleteObject& rhs) {
    lhs << "DeleteObject with n = " << rhs.getN_();
    return lhs;
}

void testImplicitConversion() {
    auto display = [](const DeleteObject& obj) {
        std::cout << obj << std::endl;
    };
    const DeleteObject obj(10);
    const DeleteObject& obj2(obj);
    display(obj);
    display(obj2);
}

int main() {
    testImplicitConversion();
    cout << "hello world" << endl;
    return 0;
}
