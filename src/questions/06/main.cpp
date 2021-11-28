//
// Created by lauro GONCALVES DA ROCHA on 22/11/2021.
//
#include "Expressions.h"

int main() {
  auto binExpFactory = new BinExpFactory();
  auto constExpFactory = new ConstExpFactory();

  auto e1 = constExpFactory->getExp(10);
  auto e2 = binExpFactory->getExp(constExpFactory->getExp(20), "+",
                                  constExpFactory->getExp(5));
  auto e3 = binExpFactory->getExp(e1, "+", e2);
  auto e4 = constExpFactory->getExp(19);
  auto e5 = binExpFactory->getExp(e4, "+", e3);
  auto e6 = binExpFactory->getExp(e5, "+", e3);
  auto e7 = binExpFactory->getExp(e5, "-", e3);

  std::cout << e1->evalExp() << std::endl;
  std::cout << e2->evalExp() << std::endl;
  std::cout << e3->evalExp() << std::endl;
  std::cout << e4->evalExp() << std::endl;
  std::cout << e5->evalExp() << std::endl;
  std::cout << e6->evalExp() << std::endl;
  std::cout << e7->evalExp() << std::endl;
  auto e8 = e7->clone();
  std::cout << e8->evalExp() << std::endl;
  e8->dump();

  try {
    auto e9 = binExpFactory->getExp(e5, "/", constExpFactory->getExp(0));
    e9->evalExp();  // exception only here, not in creation
  } catch(std::exception const& e) {
    std::cout << "Exception: " << e.what() << "\n";
  }

  try {
    auto e10 = binExpFactory->getExp(e5, "!", e3);
  } catch(std::exception const& e){
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}
