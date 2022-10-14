#include <thread>
#include "Universal.h"
#include "SeqObject.h"
#include "myThread.h"

class Counter: public SeqObject<size_t, size_t> {
public:
  Counter(size_t init) : SeqObject<size_t, size_t>(init){};
  Response<size_t> apply(Invoc invoc) override {
    if (obj < 1000000) {
      obj++;
      return Response<size_t>(true, obj);
    }
    return Response<size_t>(false, obj);
  }
};

void thread_func(Universal<Counter, size_t> universal) {
  thj::this_thread::register_id();
  Invoc invoc;
  while (universal.apply(invoc).success);
}

int main(int argc, char const *argv[]) {
  size_t THREAD_NUM = atoi(argv[1]);
  Universal<Counter, size_t> universal = Universal<Counter, size_t>(THREAD_NUM, Counter(0));
  std::thread **threads = (std::thread **)malloc(THREAD_NUM * sizeof(std::thread *));
  for (size_t i = 0; i < THREAD_NUM; i++)
    threads[i] = new std::thread(thread_func, universal);
  for (size_t i = 0; i < THREAD_NUM; i++)
    threads[i]->join();

  return 0;
}
