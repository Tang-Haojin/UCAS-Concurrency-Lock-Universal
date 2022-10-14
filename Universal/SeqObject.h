#ifndef __SEQ_OBJECT_H__
#define __SEQ_OBJECT_H__

#include "Invoc.h"
#include "Response.h"

template <typename T, typename S> class SeqObject {
public:
  SeqObject(T init) {
    obj = init;
  }
  virtual Response<S> apply(Invoc invoc) = 0;

protected:
  T obj;
};

#endif
