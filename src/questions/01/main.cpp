#include <iostream>

class A {
 public:
  A() { std::cout << "A::A()" << std::endl; }
  /* problem here : not defining a virtual-destructor will cause undefined
   * behavior if the child object is deleted using the base class*/
  ~A() { std::cout << "A::~A()" << std::endl; }
  virtual void foo() { std::cout << "A::foo()" << std::endl; }
  void bar() { std::cout << "A::bar()" << std::endl; }
};

class B : public A {
 public:
  B() { std::cout << "B::B()" << std::endl; };
  ~B() { std::cout << "B::~B()" << std::endl; };
  void foo() { std::cout << "B::foo()" << std::endl; }
};

int main() {
  A a;
  a.foo();
  a.bar();
  A* b(new B());
  b->foo();
  b->bar();
  delete b;
  return 0;
}
