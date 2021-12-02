#include <iostream>
#include <thread>

#include "SyncQueue.h"

SyncQueue<std::string> mySynchQueue;

void producer() {
  std::cout << "Pushing elements to the queue" << std::endl;
  mySynchQueue.push("Lauro");
  mySynchQueue.push("Paula");
  mySynchQueue.push("Marina");
  std::cout << "sleeping producer" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(10));
  std::cout << "awaken producer" << std::endl;
  mySynchQueue.push("Diva");
  mySynchQueue.push("Sara");
  mySynchQueue.push("Lilica");
}

void consumer() {
  std::cout << "sleeping consumer" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Popping elements of the queue" << std::endl;
  std::cout << mySynchQueue.pop() << std::endl;
  std::cout << mySynchQueue.pop() << std::endl;
  std::cout << mySynchQueue.pop() << std::endl;
  std::cout << "Popping elements of the queue without head, probably waiting"
            << std::endl;
  std::cout << mySynchQueue.pop() << std::endl;
}

void consumer2() {
  std::cout << "sleeping consumer" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(7));
  std::cout << "Popping elements of the queue viq consumer2 without head, "
               "probably waiting"
            << std::endl;
  std::cout << mySynchQueue.pop() << std::endl;
}

int main() {
  std::thread t1(producer);
  std::thread t2(consumer);
  std::thread t3(consumer2);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}