#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>

class TASlock {
public:
  TASlock() : state(false){};
  void lock() {
    while (state.exchange(true));
  }
  void unlock() {
    state = false;
  }

private:
  std::atomic_bool state;
};

TASlock lock;
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
