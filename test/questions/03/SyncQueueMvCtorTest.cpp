//
// Created by Lauro Goncalves da Rocha on 03/12/2021.
//
#include "gtest/gtest.h"
#include "questions/03/SyncQueueMvCtor.h"

class SyncQueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    q1.push(1);
    q2.push(2);
    q2.push(3);
  }

  // void TearDown() override {}

  SyncQueueMvCtor<int> q0;
  SyncQueueMvCtor<int> q1;
  SyncQueueMvCtor<int> q2;
};


TEST_F(SyncQueueTest, IsEmpty) {
  EXPECT_TRUE(q0.isEmpty());
}

TEST_F(SyncQueueTest, IsNotEmpty) {
  EXPECT_FALSE(q1.isEmpty());
}

TEST_F(SyncQueueTest, PushValue) {
  q0.push(10);
  EXPECT_FALSE(q0.isEmpty());
}

TEST_F(SyncQueueTest, PopValue) {
  EXPECT_EQ(q1.pop(),1);
}