#include <iostream>

class MyNumber {
public:
    MyNumber(int x){
        this->value = x;
        std::cout << "Conversion constructor called." << std::endl;
    }

    void showValue() {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value;
};

int main() {
    MyNumber num = 42;  // Implicit conversion using the constructor
    num.showValue();

    return 0;
}
