#ifndef __NODE_H__
#define __NODE_H__

#include "Invoc.h"
#include "Consensus.h"

class Node {
public:
  Invoc invoc;
  Consensus<Node> decideNext;
  Node *next = nullptr;
  int seq;
  Node(size_t thread_num, Invoc in = "") : invoc(in), decideNext(thread_num), seq(0){};
  static Node *max(size_t thread_num, Node **array) {
    Node *max = array[0];
    for (size_t i = 0; i < thread_num; i++)
      if (max->seq < array[i]->seq)
        max = array[i];
    return max;
  }
};

#endif
