#ifndef __CONSENSUS_H__
#define __CONSENSUS_H__

#include <stddef.h>
#include <atomic>
#include "myThread.h"

template <typename T> class ConsensusProtocal {
public:
  ConsensusProtocal(size_t thread_num) : proposed((T **)calloc(thread_num, sizeof(T *))){};
  T **proposed;
  void propose(T *value) {
    proposed[thj::this_thread::get_id()] = value;
  }
  virtual T *decide(T *value) = 0;
};

template <typename T> class Consensus: public ConsensusProtocal<T> {
public:
  Consensus(size_t thread_num) : ConsensusProtocal<T>(thread_num), r(FIRST){};
  T *decide(T *value) override {
    thread_local const size_t i(thj::this_thread::get_id());
    this->propose(value);
    size_t other_thread_id = FIRST;
    if (r.compare_exchange_strong(other_thread_id, i))
      return this->proposed[i];
    else
      return this->proposed[other_thread_id];
  }

private:
  const size_t FIRST{SIZE_MAX};
  std::atomic_size_t r;
};

#endif
