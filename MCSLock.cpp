#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>

class QNode {
public:
  bool locked = false;
  QNode *next = nullptr;
};

class MCSLock {
public:
  MCSLock() : tail(nullptr){};
  void lock() {
    QNode *qnode = myNode;
    QNode *pred = tail.exchange(qnode);
    if (pred != nullptr) {
      qnode->locked = true;
      pred->next = qnode;
      while (qnode->locked);
    }
  }
  void unlock() {
    QNode *qnode = myNode;
    if (qnode->next == nullptr) {
      if (tail.compare_exchange_strong(qnode, nullptr))
        return;
      while (myNode->next == nullptr);
    }
    myNode->next->locked = false;
    myNode->next = nullptr;
  }

private:
  std::atomic<QNode *> tail;
  static thread_local QNode *myNode;
};

thread_local QNode *MCSLock::myNode{new QNode};

MCSLock lock;
int counter = 0;

void thread_func() {
  bool working = true;
  while (working) {
    lock.lock();
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
