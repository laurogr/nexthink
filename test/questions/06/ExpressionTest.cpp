//
// Created by Lauro Goncalves da Rocha on 05/12/2021.
//

#include "gtest/gtest.h"
#include "questions/06/ExpressionsSharedPtr.h"


class ExpressionTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto binExpFactory = new BinExpFactory();
    auto constExpFactory = new ConstExpFactory();

    e1 = constExpFactory->getExp(10);
    e2 = binExpFactory->getExp(constExpFactory->getExp(20), "+",
                                    constExpFactory->getExp(5));
    e3 = binExpFactory->getExp(e1, "+", e2);
    e4 = constExpFactory->getExp(19);
    e5 = binExpFactory->getExp(e4, "-", e3);
    e6 = binExpFactory->getExp(e5, "*", e3);
    e7 = binExpFactory->getExp(e5, "/", constExpFactory->getExp(4));
    e8 = e3->clone();
    e9 = binExpFactory->getExp(constExpFactory->getExp(5),"/",constExpFactory->getExp(0));

  }
  std::shared_ptr<Expression> e1,e2,e3,e4,e5,e6,e7,e8,e9;

  // void TearDown() override {}
};

TEST_F(ExpressionTest, getValueConst) {
  ASSERT_EQ(e1->evalExp(),10);
}

TEST_F(ExpressionTest, SumConstExpValues) {
  ASSERT_EQ(e2->evalExp(),25);
}

TEST_F(ExpressionTest, SumExpValues) {
  ASSERT_EQ(e3->evalExp(),35);
}

TEST_F(ExpressionTest, SubtractExpValues) {
  ASSERT_EQ(e5->evalExp(),-16);
}

TEST_F(ExpressionTest, MultExpValues) {
  ASSERT_EQ(e6->evalExp(),-560);
}

TEST_F(ExpressionTest, DivExpValues) {
  ASSERT_EQ(e7->evalExp(),-4);
}

TEST_F(ExpressionTest, CloneExp) {
  ASSERT_EQ(e8->evalExp(),35);
}

TEST_F(ExpressionTest, ZeroDiv) {
  EXPECT_ANY_THROW(e9->evalExp());
}
