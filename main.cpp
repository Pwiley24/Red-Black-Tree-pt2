//This program creates a binary tree of nodes that stores integer values.
//you can search to see if a number exists in the tree, add numbers with a file or by number,
//delete values from the tree, and display the tree in the terminal.
//author: Paige Wiley
//date: 04-09-2023


#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void adjustNode(Node* current, Node* &root, char side);
void add_to_tree(Node* value, Node* current, Node* &root, char &side, int count);
void display_tree(Node* current, int depth);
Node* search_tree(Node* current, int value);
void delete_from_tree(Node* position, Node* parent, Node* &root);
Node* find_left_or_right(Node* current, char direction);
void rightRotate(Node* top, Node* &root);
void leftRotate(Node* top, Node* &root);

int main(){
  char input[20];
  Node* root = NULL;
  bool running = true;
  char side;

  while(running){
    cout << "Enter a command (ADD, DELETE, SEARCH, DISPLAY, QUIT):" << endl;
    cin.get(input, 20);
    cin.ignore(20, '\n');

    if(strcmp(input, "ADD") == 0){//adding values
      cout << "Add by FILE or NUMBER?" << endl;
      cin.get(input, 20);
      cin.ignore(20, '\n');

      if(strcmp(input, "FILE") == 0){//adding by file
	char fileName[50];
	cout << "What is the name of the file?" << endl;
	cin.get(fileName, 50);
	cin.ignore(50, '\n');

	vector<int> fileNums;
	ifstream file("number.txt");
	if(file.is_open()){
	  int a;
	  while(file >> a){
	    fileNums.push_back(a);
	  }
	  file.close();
	}else{
	  cout << "File could not be opened!" << endl;
	}

	vector<int>::iterator ptr;
	for(ptr = fileNums.begin(); ptr < fileNums.end(); ptr++){//add all values
	  Node* newNode = new Node();
	  newNode->setValue(*ptr);
	  add_to_tree(newNode, root, root, side, 0);
	  //the node has been established. Adjust nodes to match red tree black tree
	  cout << "Current: " << newNode->getValue() << endl;
	  adjustNode(newNode, root, side);
	}
      }else if(strcmp(input, "NUMBER") == 0){ //adding by number
	int number;
	cout << "What is the number you'd like to add?" << endl;
	cin >> number;
	cin.ignore(10, '\n');

	Node* newNode = new Node();
	newNode->setValue(number);
	add_to_tree(newNode, root, root, side, 0);
	//the node has been established. Adjust nodes to match red tree black tree
	adjustNode(newNode, root, side);
      }
    }else if(strcmp(input, "DELETE") == 0){//delete a value in tree
      int number;
      cout << "What number would you like to delete?" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      Node* position = search_tree(root, number);
      if(position != NULL){//value exists to delete
	delete_from_tree(position, position->getParent(), root); 
      }else{
        cout << "Number doesn't exist in tree." << endl;
      }
      
    }else if(strcmp(input, "DISPLAY") == 0){//print the tree
      if(root != NULL){
        display_tree(root, 0);
      }else{
        cout << "Nothing to display" << endl;
      }
      
    }else if(strcmp(input, "SEARCH") == 0){//search for a number in tree
      int number;
      cout << "Enter a number to search" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      if(search_tree(root, number) != NULL){//if true say so!
	cout << "Number exists in tree :)" << endl;
      }else{
	cout << "Number doesn't exist in tree :(" << endl;
      }
    }else if(strcmp(input, "QUIT") == 0){//quit program
      running = false;
    }
  }
  return 0;
}


//delete a number from the tree
//position is the node you are going to delete
//gathered from searching the tree for the node.
//parent = parent of position
//Note: parent can be null -> root of tree is being deleted
//cases: 0 children, 1 child, 2 children
void delete_from_tree(Node* position, Node* parent, Node* &root){
  //Case one:
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
    if(parent == NULL){//deleting the root with no children - head is null now
      root = NULL; 
    }else if(parent->getLeft() == position){//set parent left to null
      parent->setLeft(NULL);
    }else{
      parent->setRight(NULL);
    }
    delete position;


       //Case two:
   }else if((position->getLeft() != NULL &&
	   position->getRight() == NULL) ||
	   (position->getLeft() == NULL &&
	    position->getRight() != NULL)){//one child to left/right of position



    if(position->getLeft() != NULL){//set left value of position to new position and delete
      if(parent == NULL){//deleting root and replacing with left child
	position->getLeft()->setParent(NULL);
        root = position->getLeft();
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(position->getLeft());
        }else{
  	  parent->setRight(position->getLeft());
        }
        position->getLeft()->setParent(parent);
      }
    }else{//setting right of position
      if(parent == NULL){//deleting root and replacing with right child
        position->getRight()->setParent(NULL);
        root = position->getRight();
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(position->getRight());
        }else{
	  parent->setRight(position->getRight());
        }
        position->getRight()->setParent(parent);
      }
    }
    delete position;

    
    //Case three:
  }else if(position->getLeft() != NULL &&
	   position->getRight() != NULL){//two children - find farthest left position right's left

    //STORE RIGHT ONE
    Node* rightOne = position->getRight();

    //FIND LEFT MOST
    Node* leftMost = find_left_or_right(rightOne, 'L');

    position->setValue(leftMost->getValue());
    delete_from_tree(leftMost, leftMost->getParent(), root);
    
    
  }
}


//find the left most or right most node from a starting position
Node* find_left_or_right(Node* current, char direction){
  if(direction == 'L'){//find left most
    if(current->getLeft() != NULL){//more to the left - recursion
      return find_left_or_right(current->getLeft(), 'L');
    }else{//return current
      return current;
    }
  }else if(direction == 'R'){//find right most
    if(current->getRight() != NULL){
      return find_left_or_right(current->getRight(), 'R');
    }else{//return current
      return current;
    }
  }
  return NULL;
}

//searches the tree by binary searching
Node* search_tree(Node* current, int value){
  if(current->getValue() == value){//value matches the head value
    return current;
  }else{ //continue search
    if(current->getValue() <= value &&
       current->getRight() != NULL){//the value must be to the right
      return search_tree(current->getRight(), value);
    }else if(current->getValue() > value &&
	     current->getLeft() != NULL){//value must be to the left
      return search_tree(current->getLeft(), value);
    }
  }
  return NULL;
}

//prints numbers from the tree in order
void display_tree(Node* current, int depth){
  if(current->getRight() != NULL){ //still a value to the left
    display_tree(current->getRight(), (depth+1));
  }
  for(int i = 0; i < depth; i++){
    cout << '\t';
  }
  
  cout << current->getColor() << current->getValue() << endl;

  if(current->getLeft() != NULL){//still value to the right
    display_tree(current->getLeft(), (depth+1));
  }
}




//add number value to tree
void add_to_tree(Node* value, Node* current, Node* &root, char &side, int count){
  if(root == NULL){//first value
    root = value;
    root->setColor('b');
  }else{//not first
    if(current->getValue() > value->getValue()){ //value goes to left
      if(count == 0){//determines which side of tree value is on
	cout << "value going to left" << endl;
	side = 'l';
      }
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
	value->setParent(current);
	value->setColor('r');
      }else{ //keep going down left of head
	count++;
	add_to_tree(value, current->getLeft(), root, side, count);
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(count == 0){//determines which side of tree value is on
	cout << "value going to right" << endl;
	side = 'r';
      }
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
	value->setParent(current);
	value->setColor('r');
      }else{//keep going down right of head
	count++;
	add_to_tree(value, current->getRight(), root, side, count);
      }
    }
  }
}

//readjusts the nodes in the tree to follow the R-B rules
//no two reds in a row
//current always starts as red
//case 1: new node is root
//case 2: parent node is red and root
//case 3: parent and uncle nodes are red
//case 4: parent node is red, uncle node is black/NULL, inserted node is "left grandchild"
//case 5: parent node is red, uncle node is black/NULL, inserted node is "right grandchild"
void adjustNode(Node* current, Node* &root, char side){
  if(current != root){//not the root -> cases 2-4
    Node* grand = NULL;
    Node* parent = NULL;
    Node* uncle = NULL;

    cout << "current: " << current->getValue() << endl;
    if(current->getParent() != NULL){
      parent = current->getParent();
      cout << "parent: " << current->getParent()->getValue() << endl;

      //can only have grandparent if you have a parent:
      if(current->getGrand(parent) != NULL){
	grand = current->getGrand(parent);
	cout << "grand: " << current->getGrand(parent)->getValue() << endl;

	//can only have unlce if you have grandparent:
	if(current->getUncle(grand, parent) != NULL){
	  cout << "UNLCE: " << current->getUncle(grand, parent)->getValue() << endl;
	  uncle = current->getUncle(grand, parent);
	}
      }
    }
   
    if(parent != NULL &&
       parent->getColor() == 'r' &&
       parent == root){//case 2 (parent and root are red)
      //CASE 2: recolor root black
      cout << "Case 2" << current->getValue() << endl;
      root->setColor('b');
    }else if(parent->getColor() == 'r' &&
	     uncle != NULL &&
	     uncle->getColor() == 'r'){//case 3 (parent and uncle are red)
      //FOR CASE 3: recolor parent and uncle black. Grandparent becomes red
      cout << "case 3: " << current->getValue() << endl;
      parent->setColor('b');
      uncle->setColor('b');
      grand->setColor('r');
      //CASE 3: check up the tree with grandparent as current if grandparent's parent is red
      if(grand != NULL &&
	 grand->getParent() != NULL &&
	 grand->getParent()->getColor() == 'r'){
	adjustNode(grand, root, side);
      }
    }else if(parent != NULL &&
	     parent->getColor() == 'r' &&
	     (uncle == NULL || uncle->getColor() == 'b') &&
	     parent->getLeft() == current){//case 4 (parent=red, uncle=black or null, current="left grandchild"-->left of parent)
      cout << "case 4: " << current->getValue() << endl;
      //FOR CASE 4: rotate current to the right around parent
      if(grand != NULL){//should not be equal to null but good to check
	if(side == 'l'){//if on the left side, rotate left then right
	  cout << "left side" << endl;
	  rightRotate(grand, root);
	  
	}else{//if on the right side, rotate right only
	  cout << "right side" << endl;
	  rightRotate(grand, root);
	  leftRotate(grand, root);
	}

      }else{
	cout << "null grand on case 4!" << endl;
      }       
     

      
    }else if(parent->getColor() == 'r' &&
	     (uncle == NULL ||
	      uncle->getColor() == 'b') &&
	     parent->getRight() == current){//case 5 (same as case 4 but current is right of parent
      //FOR CASE 5: rotate left like in case 4
      cout << "case 5: " << current->getValue() << endl;
      if(side == 'l'){//if on left side of tree, rotate left then right
	leftRotate(grand, root);
	rightRotate(grand, root);
      }else {//if on the right side of tree, rotate left only
	leftRotate(grand, root);
      }
      
      
    }
  }else{//case 1: make sure root is black
    root->setColor('b');
    cout << "root case: " << current->getValue() << endl;
  }

  if(root->getColor() != 'b'){//ensure that root is always black
    root->setColor('b');
  }

}

/*
//rotate right around a node:   GRAND, PARENT, CURRENT
void rightRotate(Node* top, Node* middle, Node* bottom){
  /*
  if(top->getRight() == middle){//dealing with right side of tree
    top->setRight(bottom);
    bottom->setRight(middle);
    
  }else if(top->getLeft() == middle){//dealing with left side of tree
    top->setLeft(bottom);
    bottom->setLeft(middle);
  }

  bottom->setParent(top);
  middle->setParent(bottom);
  middle->setLeft(NULL);
  
  
  
}
*/

void rightRotate(Node* top, Node* &root){
  Node* bottom = top->getLeft();

  top->setLeft(bottom->getRight());

  if(bottom->getRight() != NULL){
    bottom->getRight()->setParent(top);
  }

  if(top->getParent() != NULL){//set the parent of bottom to the top's parent
    bottom->getParent()->setParent(top->getParent());
  }else{//else dealing with the root
    bottom->setParent(NULL);
  }

  if(top->getParent() != NULL){
    root = bottom;
  }else if(top == top->getParent()->getRight()){//top is a right child
    top->getParent()->setRight(bottom);
  }else{//top is left child
    top->getParent()->setLeft(bottom);
  }

  bottom->setRight(top);
  top->setParent(bottom);

}


void leftRotate(Node* top, Node* &root){
  Node* bottom = top->getRight();

  top->setRight(bottom->getLeft());

  if(bottom->getLeft() != NULL){
    bottom->getLeft()->setParent(top);
  }

  if(top->getParent() != NULL){//set parent of bottom to top's parent
    bottom->getParent()->setParent(top->getParent());
  }else{//dealing with the root
    bottom->setParent(NULL);
  }

  if(top->getParent() == NULL){
    root = bottom;
  }else if(top == top->getParent()->getLeft()){
    top->getParent()->setLeft(bottom);
  }else{
    top->getParent()->setRight(bottom);
  }

  bottom->setLeft(top);
  top->setParent(bottom);
  
}

/*
//rotate left around a node:
void leftRotate(Node* top, Node* middle, Node*  root){

  if(top->getParent() != NULL){ //not dealing with the root
    if(
    

  }else{ //dealing with the root

  }

  /*

  if(top->getParent() != NULL){
   if(top->getParent()->getRight() == top){//on right side of tree. Set right to current
     top->getParent()->setRight(middle);

   }else if(top->getParent()->getLeft() == top){//on left side of tree. Set left to current
     top->getParent()->setRight(middle);
   }

   middle->setParent(top->getParent());
   middle->setLeft(top);
   top->setParent(middle);
   top->setRight(NULL);

   //recolor the nodes properly:
   middle->setColor('b');
   top->setColor('r');
  }else{//top is the root --> must push top to be middle's left or right side of tree
    cout << "top is root" << endl;
    cout << "top: " << top->getValue() << endl;
    cout << "middle: " << middle->getValue() << endl;
    cout << "root: " << root->getValue() << endl;
    //on left side of tree:
    if(top->getLeft() == middle){
      top->setLeft(NULL);
      middle->setRight(top);
    }else if(top->getRight() == middle){ //on right side of tree
      top->setRight(NULL);
      middle->setLeft(top);
    }
    //done regardless of side
    root = middle;
    middle->setParent(NULL);
    top->setParent(middle);

    //recolor nodes properly:
    middle->setColor('b');
    top->setColor('r');

    cout << "new root: " << root->getValue() << endl;
    
  }
  
}
*/
