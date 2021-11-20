#include <iostream>

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
