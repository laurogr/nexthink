//
// Created by lauro GONCALVES DA ROCHA on 22/11/2021.
//
#include <iostream>

class Expression {
 public:
  virtual int evalExp() = 0;
  virtual Expression *clone() = 0;
  virtual void dump() = 0;
};

class ConstExpression : public Expression {
  int val;
  ConstExpression(int value) : val(value) {}

 public:
  int evalExp() override { return this->val; }
  Expression *clone() override { return new ConstExpression(this->val); }
  void dump() override { std::cout << val; };

  friend class ConstExpFactory;
};

class BinExpression : public Expression {
 protected:
  Expression *left;
  Expression *right;
  std::string operation;

  BinExpression(Expression *left, std::string operation, Expression *right)
      : left(left), operation(operation), right(right){};

  // TODO : add file support to dump
  void dump() {
    std::cout << "(";
    left->dump();
    std::cout << this->operation;
    right->dump();
    std::cout << ")";
  }
};

class SumExpression : public BinExpression {
  friend class BinExpFactory;
  SumExpression(Expression *left, std::string operation, Expression *right)
      : BinExpression(left, operation, right) {}

 public:
  int evalExp() override { return left->evalExp() + right->evalExp(); }

  Expression *clone() override {
    return new SumExpression(left->clone(), operation, right->clone());
  }
};

class SubtractExpression : public BinExpression {
  friend class BinExpFactory;
  SubtractExpression(Expression *left, std::string operation, Expression *right)
      : BinExpression(left, operation, right) {}
 public:
  int evalExp() override { return left->evalExp() - right->evalExp(); }

  Expression *clone() override {
    return new SubtractExpression(left->clone(), operation, right->clone());
  }
};

class MultiExpression : public BinExpression {
  friend class BinExpFactory;
  MultiExpression(Expression *left, std::string operation, Expression *right)
      : BinExpression(left, operation, right) {}
 public:
  int evalExp() override { return left->evalExp() * right->evalExp(); }

  Expression *clone() override {
    return new MultiExpression(left->clone(), operation, right->clone());
  }
};

class DivExpression : public BinExpression {
  friend class BinExpFactory;
  DivExpression(Expression *left, std::string operation, Expression *right)
      : BinExpression(left, operation, right) {}
 public:
  int evalExp() override {
    // TODO : DEVIDED BY ZERO
    auto rightEval = right->evalExp();
    auto leftEval = left->evalExp();
    if (rightEval == 0) {
      throw std::runtime_error("EXCEPTION : DIVISION BY ZERO");
    }
    return leftEval / rightEval;
  }

  Expression *clone() override {
    return new DivExpression(left->clone(), operation, right->clone());
  }
};

class BinExpFactory {
 public:
  Expression *getExp(Expression *leftExp, std::string operation,
                     Expression *rightExp) {
    // TODO : INVALID EXPRESSION
    if (!leftExp or !rightExp)
      throw std::runtime_error("EXCEPTION : INVALID EXPRESSION");

    if (operation == "+")
      return new SumExpression(leftExp, operation, rightExp);
    else if (operation == "-")
      return new SubtractExpression(leftExp, operation, rightExp);
    else if (operation == "*")
      return new MultiExpression(leftExp, operation, rightExp);
    else if (operation == "/")
      return new DivExpression(leftExp, operation, rightExp);
    else  // TODO : UNSUPORTED OP
      throw std::runtime_error("EXCEPTION : UNSUPORTED OP");
  }
};

class ConstExpFactory {
 public:
  Expression *getExp(int value) { return new ConstExpression(value); }
};

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
  } catch (...) {
    std::cout << "Exception division by zero" << std::endl;
  }

  try {
    auto e10 = binExpFactory->getExp(e5, "!", e3);
  } catch (...) {
    std::cout << "Exception unsu op" << std::endl;
  }

  return 0;
}
