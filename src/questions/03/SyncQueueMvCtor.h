//
// Created by lauro GONCALVES DA ROCHA on 21/11/2021.
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
class SyncQueueMvCtor {
 private:
  std::shared_ptr<struct smartNode<T>> head = { nullptr };
  std::shared_ptr<struct smartNode<T>> tail = { nullptr };

  std::mutex head_mutex;
  std::mutex tail_mutex;
  std::condition_variable conditionVariable;

 public:
  SyncQueueMvCtor() = default;
  ~SyncQueueMvCtor() = default;
  SyncQueueMvCtor(const SyncQueueMvCtor &);
  SyncQueueMvCtor(SyncQueueMvCtor &&);


  T pop();  // Pops an element from the queue. It blocks if the queue is empty.
  void push(const T& item);  // Pushes an element into the queue
  bool isEmpty() { return (!head) ? true : false; }
};

template <typename T>
SyncQueueMvCtor<T>::SyncQueueMvCtor(SyncQueueMvCtor<T> &&myQueue) {
  this->head = myQueue.head;
  this->tail = myQueue.tail;

  myQueue.head = nullptr;
  myQueue.tail = nullptr;
}

template <typename T>
SyncQueueMvCtor<T>::SyncQueueMvCtor(const SyncQueueMvCtor<T> &origin) {
  auto node = origin.head;
  while(node) {
    this->push(node->value);
    node = node->next;
  }

}

template <typename T>
void SyncQueueMvCtor<T>::push(const T& item) {
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
T SyncQueueMvCtor<T>::pop() {
  if (!head) {
    std::unique_lock<std::mutex> lockEmptyHead(head_mutex);
    while (!head) conditionVariable.wait(lockEmptyHead);
  }

  std::lock_guard<std::mutex> lock_head(head_mutex);
  T ret = head->value;
  this->head = this->head->next;

  return ret;
}