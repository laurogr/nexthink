//
// Created by lauro GONCALVES DA ROCHA on 22/11/2021.
//

#include <iostream>

class Expression {
 public:
  virtual int evalExp() = 0;
  virtual Expression* clone() = 0;
  //TODO : dump
};

class ConstExpression : public Expression {
  int val;
  ConstExpression(int value) : val(value) {}

 public:
  static ConstExpression* create(int type) { return new ConstExpression(type); }
  int evalExp() { return this->val; }
  Expression* clone() { return new ConstExpression(this->val); }
};

class BinExpression : public Expression {
 protected:
  Expression* left;
  Expression* right;
  std::string operation;

  BinExpression(Expression* left, std::string operation, Expression* right)
      : left(left), operation(operation), right(right){};
};

class SumExpression : public BinExpression {
  SumExpression(Expression* left, std::string operation, Expression* right)
      : BinExpression(left, operation, right) {}

 public:
  static SumExpression* create(Expression* left, std::string operation,
                               Expression* right) {
    return new SumExpression(left, operation, right);
  }
  int evalExp() {
    int sum = left->evalExp() + right->evalExp();
    return sum;
  }
  Expression* clone() {
    return new SumExpression(left->clone(), operation, right->clone());
  }
};

class SubtractExpresion : public BinExpression {
 public:
  SubtractExpresion(Expression* left, std::string operation, Expression* right)
      : BinExpression(left, operation, right) {}

  int evalExp() {
    int sum = left->evalExp() - right->evalExp();
    return sum;
  }
  Expression* clone() {
    return new SubtractExpresion(left->clone(), operation, right->clone());
  }
};

class MultiExpression : public BinExpression {
 public:
  MultiExpression(Expression* left, std::string operation, Expression* right)
      : BinExpression(left, operation, right) {}

  int evalExp() { return left->evalExp() * right->evalExp(); }

  Expression* clone() {
    return new MultiExpression(left->clone(), operation, right->clone());
  }
};

class DivExpression : public BinExpression {
 public:
  DivExpression(Expression* left, std::string operation, Expression* right)
      : BinExpression(left, operation, right) {}

  int evalExp() {
    //TODO : rightEval == 0
    int sum = left->evalExp() / right->evalExp();
    return sum;
  }
  Expression* clone() {
    return new DivExpression(left->clone(), operation, right->clone());
  }
};

class BinExpFactory {
 public:
  Expression* getExp(Expression* leftExp, std::string operation,
                     Expression* rightExp) {
    if (operation == "+")
      return SumExpression::create(leftExp, operation, rightExp);
    else if (operation == "-")
      return new SubtractExpresion(leftExp, operation, rightExp);
    else if (operation == "*")
      return new MultiExpression(leftExp, operation, rightExp);
    else if (operation == "/")
      return new DivExpression(leftExp, operation, rightExp);
    else
      std::cout << "ERROR : NOT SUPPORTED" << std::endl;
  }
};

class ConstExpFactory {
 public:
  Expression* getExp(int value) { return ConstExpression::create(value); }
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



  return 0;
}
