#ifndef RBTREE_H 
#define RBTREE_H

enum COLOR { RED=1, BLACK, DB_BLACK };

class Node
{
public:
	Node(int,COLOR);
	Node(int);
	~Node();


	int getKey() const { return key; }
	void setKey(int key) { this->key = key; }
	COLOR getColor() const { return color; }
	void setColor(COLOR c) { color = c; }
	//fields also public
	
	Node * left;
	Node * right;
	Node * parent;
private:
	int key;
	COLOR color;
};

extern Node * nil;   //global variable, has the role of a sentinel

void insert(Node * & , Node * );
void printTree(Node *, int);
void leftRot(Node * &, Node *);
void rightRot(Node * &, Node *);
void rbInsert(Node * &, Node *);
Node * successor(Node *);
void rbDelete(Node * & , Node * );
void rbFix(Node * & , Node * );
Node * search(Node *, int);


#endif
