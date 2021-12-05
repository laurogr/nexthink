//
// Created by Lauro Goncalves da Rocha on 28/11/2021.
//
#pragma once
#include <fstream>
#include <iostream>
#include <sstream>

//rule of 0g
class Expression {
 public:
  virtual int evalExp() = 0;
  virtual std::shared_ptr<Expression> clone() = 0;
  virtual std::string genExpString() const = 0;
};

class ExpressionDumper {
 public:
  void dump(const std::string& fileName, const std::shared_ptr<Expression>& expression) {
    std::ofstream f(fileName);
    if (f.is_open()) f << expression->genExpString();
    f.close();
  }
};

class ConstExpression : public Expression {
  int val;

 public:
  ConstExpression(int value) : val(value) {}
  int evalExp() override { return val; }
  std::shared_ptr<Expression> clone() override {
    return std::make_shared<ConstExpression>(val);
  }
  std::string genExpString() const override { return std::to_string(val); }
};

class BinExpression : public Expression {
 protected:
  std::shared_ptr<Expression> left;
  std::shared_ptr<Expression> right;
  std::string operation;

  BinExpression(std::shared_ptr<Expression> left, std::string operation,
                std::shared_ptr<Expression> right)
      : left(std::move(left)),
        operation(std::move(operation)),
        right(std::move(right)){};

  std::string genExpString() const override {
    std::stringstream ss;
    ss << "(" << left->genExpString();
    ss << operation;
    ss << right->genExpString() << ")";

    return ss.str();
  }
};

class SumExpression : public BinExpression {
 public:
  SumExpression(std::shared_ptr<Expression> left, std::string operation,
                std::shared_ptr<Expression> right)
      : BinExpression(std::move(left), std::move(operation), std::move(right)) {
  }

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
      : BinExpression(std::move(left), std::move(operation), std::move(right)) {
  }

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
      : BinExpression(std::move(left), std::move(operation), std::move(right)) {}

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
      : BinExpression(std::move(left), std::move(operation), std::move(right)) {}

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
