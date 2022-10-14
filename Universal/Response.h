#ifndef __RESPONSE_H__
#define __RESPONSE_H__

template <typename T> class Response {
public:
  bool success;
  T value;
  Response(bool init, T value) {
    this->success = init;
    this->value = value;
  }
};

#endif
