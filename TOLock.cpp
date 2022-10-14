#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

class QNode {
public:
  QNode *pred = nullptr;
};

class TOLock {
public:
  TOLock() : tail(nullptr){};
  bool lock(int64_t time) {
    auto startTime = std::chrono::steady_clock::now();
    QNode *qnode = new QNode;
    myNode = qnode;
    QNode *myPred = tail.exchange(qnode);
    if (myPred == nullptr || myPred->pred == AVAILABLE)
      return true;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    while (duration.count() < time) {
      QNode *predPred = myPred->pred;
      if (predPred == AVAILABLE)
        return true;
      else if (predPred != nullptr)
        myPred = predPred;
    }
    QNode *tempNode = qnode;
    if (!tail.compare_exchange_strong(tempNode, myPred))
      qnode->pred = myPred;
    return false;
  }
  void unlock() {
    QNode *qnode = myNode;
    if (!tail.compare_exchange_strong(qnode, nullptr))
      myNode->pred = AVAILABLE;
  }

private:
  std::atomic<QNode *> tail;
  static thread_local QNode *myNode;
  static QNode *AVAILABLE;
};

thread_local QNode *TOLock::myNode{};
QNode *TOLock::AVAILABLE{new QNode};

TOLock lock;
int counter = 0;

void thread_func() {
  bool working = true;
  while (working) {
    while (!lock.lock(100));
    if (counter == 1000000)
      working = false;
    else
      counter++;
    lock.unlock();
  }
}

int main(int argc, char const *argv[]) {
  int THREAD_NUM = atoi(argv[1]);
  std::thread **threads = new std::thread *[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++)
    threads[i] = new std::thread(thread_func);
  for (int i = 0; i < THREAD_NUM; i++)
    threads[i]->join();
  std::cout << "counter: " << counter << std::endl;
  return 0;
}
