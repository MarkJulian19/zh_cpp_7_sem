#include <iostream>
#include <memory>
#include <typeinfo>
#include <string>

class Any {
    struct BaseHolder {
        virtual ~BaseHolder() {}
        virtual std::unique_ptr<BaseHolder> clone() const = 0;
        virtual const std::type_info& type() const = 0; 
    };

    template<typename T>
    struct Holder : BaseHolder {
        T value;
        Holder(const T& v) : value(v) {}
        std::unique_ptr<BaseHolder> clone() const override {
            return std::make_unique<Holder>(value);
        }
        const std::type_info& type() const override { 
            return typeid(T);
        }
    };

    std::unique_ptr<BaseHolder> ptr;

public:
    Any() : ptr(nullptr) {}

    Any(const Any& other) : ptr(nullptr) {
        if (other.ptr)
            ptr = other.ptr->clone();
    }

    Any(Any&& other) noexcept : ptr(std::move(other.ptr)) {}

    template<typename T>
    Any(const T& value) : ptr(std::make_unique<Holder<T>>(value)) {}

    Any& operator=(const Any& other) {
        if (this != &other) {
            ptr.reset();
            if (other.ptr)
                ptr = other.ptr->clone();
        }
        return *this;
    }

    Any& operator=(Any&& other) noexcept {
        if (this != &other) {
            ptr = std::move(other.ptr);
        }
        return *this;
    }

    template<typename T>
    T get() const {
        if (!ptr) {
            throw std::runtime_error("Empty Any");
        }
        if (ptr->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<Holder<T>*>(ptr.get())->value;
    }
};

int main() {
    Any a(5); 
    Any b;    

    try {
        // Корректный вызов
        int x = a.get<int>(); // Should succeed
        std::cout << x << std::endl;

        // Некорректный тип
        std::string s = a.get<std::string>(); 
    }
    catch (const std::bad_cast& e) {
        std::cout << "Type cast error: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }

    try {
        // Пустой Any
        int y = b.get<int>(); 
        std::cout << y << std::endl;
    }
    catch (const std::bad_cast& e) {
        std::cout << "Type cast error: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }

    return 0;
}