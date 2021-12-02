//
// Created by Lauro Goncalves da Rocha on 28/11/2021.
//

#pragma once
#include <iostream>

class Expression {
 public:
  virtual int evalExp() = 0;
  virtual Expression *clone() = 0;
  virtual void dump() = 0;
  virtual ~Expression() = default;
};


class ConstExpression : public Expression {
  int val;
  ConstExpression(int value) : val(value) {}
  ~ConstExpression() = default;

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
  ~BinExpression() {
    if(left) {
      delete left;
      delete right;
    }
  }

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
  ~SumExpression()=default;
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
  ~SubtractExpression() = default;

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
  ~MultiExpression() = default;
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
  ~DivExpression() = default;
 public:
  int evalExp() override {
    auto rightEval = right->evalExp();
    auto leftEval = left->evalExp();
    if (rightEval == 0) {
      throw std::runtime_error("DIVISION BY ZERO");
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

    if (!leftExp or !rightExp)
      throw std::runtime_error("INVALID EXPRESSION");

    if (operation == "+")
      return new SumExpression(leftExp, operation, rightExp);
    else if (operation == "-")
      return new SubtractExpression(leftExp, operation, rightExp);
    else if (operation == "*")
      return new MultiExpression(leftExp, operation, rightExp);
    else if (operation == "/")
      return new DivExpression(leftExp, operation, rightExp);
    else
      throw std::runtime_error("UNSUPPORTED OPERATION");
  }
};

class ConstExpFactory {
 public:
  Expression *getExp(int value) { return new ConstExpression(value); }
};
