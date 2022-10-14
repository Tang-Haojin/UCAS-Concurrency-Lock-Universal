#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>

class ALock {
public:
  ALock(size_t thread_num) : flags(new bool[thread_num]{true}), next(0), thread_num(thread_num){};
  void lock() {
    mySlot = next++;
    while (!flags[mySlot % thread_num]);
    flags[mySlot % thread_num] = false;
  }
  void unlock() {
    flags[(mySlot + 1) % thread_num] = true;
  }

private:
  bool *flags;
  std::atomic_size_t next;
  static thread_local size_t mySlot;
  const size_t thread_num;
};

thread_local size_t ALock::mySlot{0};

ALock *lock;
int counter = 0;

void thread_func() {
  bool working = true;
  while (working) {
    lock->lock();
    if (counter == 1000000)
      working = false;
    else
      counter++;
    lock->unlock();
  }
}

int main(int argc, char const *argv[]) {
  int THREAD_NUM = atoi(argv[1]);
  lock = new ALock(THREAD_NUM);
  std::thread **threads = new std::thread *[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++)
    threads[i] = new std::thread(thread_func);
  for (int i = 0; i < THREAD_NUM; i++)
    threads[i]->join();
  std::cout << "counter: " << counter << std::endl;
  return 0;
}
