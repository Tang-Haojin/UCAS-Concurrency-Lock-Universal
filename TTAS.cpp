#include <stdlib.h>
#include <atomic>
#include <thread>
#include <iostream>

class TTASlock {
public:
  TTASlock() : state(false){};
  void lock() {
    while (true) {
      while (state.load());
      if (!state.exchange(true))
        return;
    }
  }
  void unlock() {
    state = false;
  }

private:
  std::atomic_bool state;
};

TTASlock lock;
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
