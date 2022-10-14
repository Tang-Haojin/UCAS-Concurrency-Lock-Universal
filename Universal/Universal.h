#ifndef __UNIVERSAL_H__
#define __UNIVERSAL_H__

#include "Invoc.h"
#include "Node.h"
#include "Response.h"
#include "myThread.h"

template <typename T, typename S> class Universal {
public:
  Universal(size_t thread_num, T initval) : init(initval), thread_num(thread_num) {
    tail = new Node(thread_num);
    tail->seq = 1;
    head = new Node *[thread_num];
    for (size_t i = 0; i < thread_num; i++)
      head[i] = tail;
  }
  Response<S> apply(Invoc invoc) {
    thread_local const size_t i = thj::this_thread::get_id();
    Node *prefer = new Node(thread_num, invoc);
    while (prefer->seq == 0) {
      Node *before = Node::max(thread_num, head);
      Node *after = before->decideNext.decide(prefer);
      before->next = after;
      after->seq = before->seq + 1;
      head[i] = after;
    }
    T myObject = init;
    Node *current = tail->next;
    while (current != prefer) {
      myObject.apply(current->invoc);
      current = current->next;
    }
    auto a = myObject.apply(current->invoc);
    return a;
  }

private:
  const size_t thread_num;
  const T init;
  Node **head;
  Node *tail;
};

#endif
