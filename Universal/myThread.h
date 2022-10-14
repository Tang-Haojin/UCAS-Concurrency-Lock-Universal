#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include <mutex>

namespace thj {

class this_thread {
public:
  static size_t get_id() {
    return thread_id;
  }
  static void register_id() {
    mutex.lock();
    thread_id = num++;
    mutex.unlock();
  }

private:
  static std::mutex mutex;
  static size_t num;
  static thread_local size_t thread_id;
};

std::mutex this_thread::mutex;
size_t this_thread::num{0};
thread_local size_t this_thread::thread_id{0};

}

#endif
