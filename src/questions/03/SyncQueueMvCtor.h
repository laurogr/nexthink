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
  ~SyncQueueMvCtor() = default;  // smart pointer will be automatically managed
  SyncQueueMvCtor(const SyncQueueMvCtor &);
  SyncQueueMvCtor(SyncQueueMvCtor &&) noexcept;
  SyncQueueMvCtor<T> &operator=(const SyncQueueMvCtor &other);
  SyncQueueMvCtor<T> &operator=(SyncQueueMvCtor &&other) noexcept;

  T pop();  // Pops an element from the queue. It blocks if the queue is empty.
  void push(const T &item);  // Pushes an element into the queue
  bool isEmpty() {
    std::lock_guard<std::mutex> lock_head(head_mutex);
    return (!head);
  }
};

template <typename T>
SyncQueueMvCtor<T>::SyncQueueMvCtor(SyncQueueMvCtor<T> &&myQueue) noexcept
    : head(std::exchange(myQueue.head, nullptr)),
      tail(std::exchange(myQueue.tail, nullptr)) {}

template <typename T>
SyncQueueMvCtor<T>::SyncQueueMvCtor(const SyncQueueMvCtor<T> &origin) {
  auto node = origin.head;
  while (node) {
    this->push(node->value);
    node = node->next;
  }
}

template <typename T>
SyncQueueMvCtor<T> &SyncQueueMvCtor<T>::operator=(
    SyncQueueMvCtor<T> &&other) noexcept {
  if (this != other) {
    this->head = std::exchange(other.head, nullptr);
    this->tail = std::exchange(other.tail, nullptr);
  }
  return *this;
}

template <typename T>
SyncQueueMvCtor<T> &SyncQueueMvCtor<T>::operator=(
    const SyncQueueMvCtor<T> &other) {
  return *this = SyncQueueMvCtor(other);
}

template <typename T>
void SyncQueueMvCtor<T>::push(const T &item) {
  auto newNode = std::make_shared<struct smartNode<T>>(item);

  if (isEmpty()) {
    std::scoped_lock<std::mutex, std::mutex> lock(head_mutex, tail_mutex);
    this->head = std::move(newNode);
    this->tail = this->head;
  } else {
    std::lock_guard<std::mutex> lock_tail(tail_mutex);
    this->tail->next = std::move(newNode);
    this->tail = this->tail->next;
  }
  conditionVariable.notify_one();
}

template <typename T>
T SyncQueueMvCtor<T>::pop() {
  if (!head) {
    std::unique_lock<std::mutex> lockEmptyHead(head_mutex);
    conditionVariable.wait(lockEmptyHead,[this](){return head;});
  }

  std::lock_guard<std::mutex> lock_head(head_mutex);
  T ret = head->value;
  this->head = this->head->next;

  return ret;
}
