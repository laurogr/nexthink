//
// Created by lauro GONCALVES DA ROCHA on 20/11/2021.
//

#ifndef NEXTHINK_MYQUEUE_H
#define NEXTHINK_MYQUEUE_H

#include <iostream>

template <typename T>
struct node {
  T value;
  struct node* next;

  node(T val) : value(val), next(nullptr) {}
};

template <typename T>
class MyQueue {
 private:
  struct node<T>* head = {
    nullptr
  };
  struct node<T>* tail = {
    nullptr
  };

 public:
  T pop();  // Pops an element from the queue. It blocks if the queue is empty.
  void push(const T& item);  // Pushes an element into the queue
};

template <typename T>
void MyQueue<T>::push(const T& item) {
  if (!head) {
    this->head = new struct node<T>(item);
    this->tail = this->head;
  } else {
    this->tail->next = new struct node<T>(item);
    this->tail = this->tail->next;
  }
}

template <typename T>
T MyQueue<T>::pop() {
  if (!head) {
    return nullptr;
  }

  T ret = head->value;
  if (head == tail) {
    this->head = nullptr;
    this->tail = nullptr;
  } else {
    auto oldHead = this->head;
    this->head = this->head->next;
    delete (oldHead);
  }
  return ret;
}

#endif  // NEXTHINK_MYQUEUE_H
