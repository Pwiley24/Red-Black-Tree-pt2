#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;

class Node {
 public:
  Node();
  ~Node();
  void setValue(int newVal);
  int getValue();
  Node* getRight();
  Node* getLeft();
  void setRight(Node* newRight);
  void setLeft(Node* newLeft);
  void setParent(Node* newParent);
  Node* getParent();
  char getColor();
  void setColor(char colour);
  
  Node* getGrand(Node* parent);

  Node* getSib(Node* parent);

  Node* getUncle(Node* grand, Node* parent);

  int value;
  Node* parent;
  Node* left;
  Node* right;
  int color;

};

#endif
