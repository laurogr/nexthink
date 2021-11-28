#include "SyncQueueMvCtor.h"

int main() {

  SyncQueueMvCtor<std::string> originQueue;
  originQueue.push("Lauro");
  originQueue.push("Paula");
  originQueue.push("Diva");
  originQueue.push("Marina");
  originQueue.push("Divanice");
  originQueue.push("Adelia");

  SyncQueueMvCtor<std::string> copiedQueue(originQueue);
  std::cout << copiedQueue.pop() << std::endl;
  std::cout << copiedQueue.pop() << std::endl;
  std::cout << copiedQueue.pop() << std::endl;
  std::cout << copiedQueue.pop() << std::endl;

  SyncQueueMvCtor<std::string> destinationQueue(std::move(originQueue));


  std::cout << destinationQueue.pop() << std::endl;
  std::cout << destinationQueue.pop() << std::endl;

  std::cout << "Origin queue isEmpty? " << originQueue.isEmpty() << std::endl;

  return 0;
}
