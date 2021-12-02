//
// Created by lauro GONCALVES DA ROCHA on 20/11/2021.
//
#pragma once
#include <iostream>

template <typename T>
struct smartNode {
  T value;
  std::shared_ptr<struct smartNode<T>> next;

  smartNode(T val) : value(val), next(nullptr) {}
};

template <typename T>
class SyncQueue {
 private:
  std::shared_ptr<struct smartNode<T>> head = { nullptr };
  std::shared_ptr<struct smartNode<T>> tail = { nullptr };

  std::mutex head_mutex;
  std::mutex tail_mutex;
  std::condition_variable conditionVariable;

 public:
  T pop();  // Pops an element from the queue. It blocks if the queue is empty.
  void push(const T& item);  // Pushes an element into the queue
  bool isEmpty() { return (!head); }
};

template <typename T>
void SyncQueue<T>::push(const T& item) {
  auto newNode = std::make_shared<struct smartNode<T>>(item);

  std::lock_guard<std::mutex> lock_head(head_mutex);
  if (!head) {
    this->head = newNode;
    std::lock_guard<std::mutex> lock_tail(tail_mutex);
    this->tail = newNode;
  } else {
    std::lock_guard<std::mutex> lock_tail(tail_mutex);
    this->tail->next = newNode;
    this->tail = this->tail->next;
  }
  conditionVariable.notify_one();
}

template <typename T>
T SyncQueue<T>::pop() {
  if (!head) {
    std::cout << "pop : !head, waiting" << std::endl;
    std::unique_lock<std::mutex> lockEmptyHead(head_mutex);
    while (!head) conditionVariable.wait(lockEmptyHead);
  }

  std::lock_guard<std::mutex> lock_head(head_mutex);
  T ret = head->value;
  this->head = this->head->next;

  return ret;
}