#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>

class QNode {
public:
  QNode() : locked(true){};
  std::atomic_bool locked;
};

class CLHLock {
public:
  CLHLock() : tail(new QNode) {
    tail.load()->locked = false;
  }
  void lock() {
    myNode->locked = true;
    QNode *pred = tail.exchange(myNode);
    myPred = pred;
    while (pred->locked);
  }
  void unlock() {
    myNode->locked = false;
    myNode = myPred;
  }

private:
  std::atomic<QNode *> tail;
  static thread_local QNode *myNode, *myPred;
};

thread_local QNode *CLHLock::myNode{new QNode};
thread_local QNode *CLHLock::myPred{};

CLHLock lock;
int counter = 0;
thread_local int useless = 3;

void thread_func() {
  bool working = true;
  while (working) {
    lock.lock();
    if (counter == 1000000)
      working = false;
    else
      counter++;
    lock.unlock();
    for (int i = 0; i < 1000; i++)
      useless = (useless + 3) * useless;
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
