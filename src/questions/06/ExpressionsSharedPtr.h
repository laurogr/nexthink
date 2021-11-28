//
// Created by Lauro Goncalves da Rocha on 28/11/2021.
//

#ifndef NEXTHINK_EXPRESSIONS_H
#define NEXTHINK_EXPRESSIONS_H

#include <iostream>

class Expression {
 public:
  virtual int evalExp() = 0;
  virtual std::shared_ptr<Expression> clone() = 0;
  virtual void dump() = 0;
};

class ConstExpression : public Expression {
  int val;

 public:
  ConstExpression(int value) : val(value) {}
  int evalExp() override { return this->val; }
  std::shared_ptr<Expression> clone() override {
    return std::make_shared<ConstExpression>(this->val);
  }
  void dump() override { std::cout << val; };
};

class BinExpression : public Expression {
 protected:
  std::shared_ptr<Expression> left;
  std::shared_ptr<Expression> right;
  std::string operation;

  BinExpression(std::shared_ptr<Expression> left, std::string operation,
                std::shared_ptr<Expression> right)
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
 public:
  SumExpression(std::shared_ptr<Expression> left, std::string operation,
                std::shared_ptr<Expression> right)
      : BinExpression(left, operation, right) {}

  int evalExp() override { return left->evalExp() + right->evalExp(); }

  std::shared_ptr<Expression> clone() override {
    return std::make_shared<SumExpression>(left->clone(), operation,
                                           right->clone());
  }
};

class SubtractExpression : public BinExpression {
 public:
  SubtractExpression(std::shared_ptr<Expression> left, std::string operation,
                     std::shared_ptr<Expression> right)
      : BinExpression(left, operation, right) {}

  int evalExp() override { return left->evalExp() - right->evalExp(); }

  std::shared_ptr<Expression> clone() override {
    return std::make_shared<SubtractExpression>(left->clone(), operation,
                                           right->clone());
  }
};

class MultiExpression : public BinExpression {
 public:
  MultiExpression(std::shared_ptr<Expression> left, std::string operation,
                  std::shared_ptr<Expression> right)
      : BinExpression(left, operation, right) {}
  int evalExp() override { return left->evalExp() * right->evalExp(); }

  std::shared_ptr<Expression> clone() override {
    return std::make_shared<MultiExpression>(left->clone(), operation,
                                           right->clone());
  }
};

class DivExpression : public BinExpression {
 public:
  DivExpression(std::shared_ptr<Expression> left, std::string operation,
                std::shared_ptr<Expression> right)
      : BinExpression(left, operation, right) {}
  int evalExp() override {
    auto rightEval = right->evalExp();
    auto leftEval = left->evalExp();
    if (rightEval == 0) {
      throw std::runtime_error("DIVISION BY ZERO");
    }
    return leftEval / rightEval;
  }

  std::shared_ptr<Expression> clone() override {
    return std::make_shared<DivExpression>(left->clone(), operation,
                                             right->clone());
  }
};

class BinExpFactory {
 public:
  std::shared_ptr<Expression> getExp(std::shared_ptr<Expression> leftExp,
                                     std::string operation,
                                     std::shared_ptr<Expression> rightExp) {
    if (!leftExp or !rightExp) throw std::runtime_error("INVALID EXPRESSION");

    if (operation == "+")
      return std::make_shared<SumExpression>(leftExp, operation, rightExp);
    else if (operation == "-")
      return std::make_shared<SubtractExpression>(leftExp, operation, rightExp);
    else if (operation == "*")
      return std::make_shared<MultiExpression>(leftExp, operation, rightExp);
    else if (operation == "/")
      return std::make_shared<DivExpression>(leftExp, operation, rightExp);
    else
      throw std::runtime_error("UNSUPPORTED OPERATION");
  }
};

class ConstExpFactory {
 public:
  std::shared_ptr<ConstExpression> getExp(int value) {
     return std::make_shared<ConstExpression>(value);
  }
};

#endif  // NEXTHINK_EXPRESSIONS_H
