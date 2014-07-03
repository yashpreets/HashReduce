#include <cstdlib>
#include <iostream>
#include <cstdio>
#include "RBTree.h"

using namespace std;


Node * nil = new Node(0, BLACK);

//constructor
Node::Node(int key, COLOR color) :
key(key),
color(color),
left(NULL),
right(NULL),
parent(NULL)
{}

Node::Node(int key) :
key(key),
color(RED),
left(NULL),
right(NULL),
parent(NULL)
{}

//destructor
Node::~Node()
{
	//cout <<"Destructor called " << key <<endl;
	if (left != NULL)
		left->parent = NULL;
	if (right != NULL)
		right->parent = NULL;
	delete left;
	delete right;
}

void insert(Node * & T, Node * pNode)
{
	if (pNode == NULL)
		return;   //nothing to insert
	if (T == NULL)  //the tree is empty
	{
		T = pNode;
		T->left = T->right = T->parent = NULL;
		return;
	}
	Node * p = T;
	Node * q = NULL;
	while (p != NULL)
	{
		if (pNode->getKey() < p->getKey())
		{
			q = p;
			p = p->left;
		}
		else
		{
			q = p;
			p = p->right;
		}
	}
	//q now points to the node after which we have to insert
	if (pNode->getKey() < q->getKey())
	{
		q->left = pNode;
		pNode->parent = q;
	}
	else
	{
		q->right = pNode;
		pNode->parent = q;
	}
}

void printTree(Node * root, int counter)
{
	if (root == NULL)
		return;
	counter += 5;
	printTree(root->right, counter);
	for (int i = 0; i <= counter; i++)
		cout << " ";
	char color = root->getColor() == RED ? 'r' : 'b';
	cout << "(" << root->getKey() << "|" << color << ")\n";
	printTree(root->left, counter);
}

void leftRot(Node * & root, Node * x)
{
	if (x->right == NULL)
		return;				//we can do no left rotation
	Node * y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		root = y;
	else
	{
		if (x == x->parent->left)  //x is the left child of its parent
			x->parent->left = y;
		else					  //y is the right child of its parent	
			x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rightRot(Node * & root, Node * x)
{
	if (x->left == NULL)
		return;			//we cannot do a right rotation
	Node * y = x->left;
	x->left = y->right;
	if (y->right != NULL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		root = y;
	else
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	y->right = x;
	x->parent = y;
}

void rbInsert(Node * & root, Node * x)
{
	if (x == NULL)
		return;   //nothing to insert
	insert(root, x);
	x->setColor(RED);
	if (x->parent == NULL || x->parent->parent == NULL)  //we have inserted the root its child
	{
		root->setColor(BLACK);		//by convention the root is always BLACK
		return;
	}
	//the inserted node x is neither the root, nor any of its children
	while (x != root && x->parent->getColor() == RED)
	{
		if (x->parent == x->parent->parent->left) //x->parent is a left child
		{
			Node * y = x->parent->parent->right;  //y = the uncle of x
			COLOR colY;
			if (y == NULL)
				colY = BLACK;
			else
				colY = y->getColor();
			if (colY == RED)    //Case 1
			{
				x->parent->setColor(BLACK);
				y->setColor(BLACK);   //y cannot be NULL since it is RED
				y->parent->setColor(RED);
				x = x->parent->parent;
			}
			else  //color of y is BLACK (it could be NULL)
			{
				if (x == x->parent->right)  //Case 2
				{
					x = x->parent;
					leftRot(root, x);
				}
				//Case 3				
				x->parent->setColor(BLACK);
				x->parent->parent->setColor(RED);
				rightRot(root, x->parent->parent);
			}
		}
		else  //symmetric case when x->parent is a right child
		{
			Node * y = x->parent->parent->left;			//y = the uncle of x
			COLOR colY;
			if (y == NULL)
				colY = BLACK;
			else
				colY = y->getColor();
			if (colY == RED)
			{
				y->parent->setColor(RED);  //y cannot be null since it is RED
				y->setColor(BLACK);
				x->parent->setColor(BLACK);
				x = x->parent->parent;
			}
			else   //color of Y is BLACK;
			{
				if (x == x->parent->left)  //case 2
				{
					x = x->parent;
					rightRot(root, x);
				}
				//case 3						
				x->parent->parent->setColor(RED);
				x->parent->setColor(BLACK);
				leftRot(root, x->parent->parent);
			}
		}
	}
	root->setColor(BLACK);
}

Node * successor(Node * root)
{
	if (root == NULL)
		return NULL;
	if (root->right == NULL)
		return NULL;
	Node * p = root->right;
	while (p->left != NULL)
		p = p->left;
	return p;
}

void rbDelete(Node * & root, Node * z)
{
	if (z == NULL)
		return;			//nothing to delete
	if (root == NULL)
		return;
	if (z == root && z->left == NULL && z->right == NULL)
	{
		z->parent = NULL;
		delete z;
		root = NULL;
		return;
	}
	//there are at least 2 nodes in the tree
	Node * y = NULL;
	Node * x = NULL;
	if (z->left == NULL || z->right == NULL)
		y = z;
	else
		y = successor(z);
	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;
	if (x != NULL)
		x->parent = y->parent;
	else
	{
		x = nil;  //this is kind of a santinel, a global variable		                       
		x->parent = y->parent;
	}
	if (y->parent == NULL)
		root = x;     //this x cannot be a santinel because the root must have at least one child
	//the opposite was checked at the very beginning of the function
	else
	if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;
	if (y != z)
		z->setKey(y->getKey());
	COLOR colY = y->getColor();
	//we delete y
	y->left = y->right = y->parent = NULL;
	delete y;
	if (colY == BLACK)	//we might have violated property 4 or 3
	{
		if (x->getColor() == RED)
			x->setColor(BLACK);
		else
			rbFix(root, x);
	}
	else  //we don't need to fix, we delete x if it is a sentinel
	if (x == nil)
	{
		if (x == x->parent->left)
			x->parent->left = NULL;
		else
			x->parent->right = NULL;
		x->parent = x->left = x->right = NULL;
		x->setColor(BLACK);   //not needed, but better be safe :)
	}
}


//helper function, used to avoid checking in the code
//returns the color of p, or BLACK if p is NULL
COLOR colorOf(Node * p)
{
	if (p != NULL)
		return p->getColor();
	else
		return BLACK;
}

void removeSentinel(Node * p)
{
	if (p == nil)
	{
		if (p == p->parent->left)
			p->parent->left = NULL;
		else
			p->parent->right = NULL;
		p->parent = p->left = p->right = NULL;
		p->setColor(BLACK);     //not needed
	}
}

void rbFix(Node * & root, Node * x)
{
	Node * w = NULL;
	while (x != root && x->getColor() == BLACK)
	{
		if (x == x->parent->left)   //x is a left child
		{
			w = x->parent->right;
			if (w != NULL)
			{
				if (w->getColor() == RED)  //Case1  (brother of x is RED)
				{
					w->parent->setColor(RED);  //w cannot be NULL, since it is RED
					w->setColor(BLACK);
					leftRot(root, x->parent);
					w = x->parent->right;
				}
				else //w is BLACK => Cases 2, 3 or 4
				{
					if (colorOf(w->left) == BLACK && colorOf(w->right) == BLACK)  //case 2
					{
						w->setColor(RED);
						Node * p = x;
						x = x->parent;
						x->setColor(BLACK);
						removeSentinel(p);
					}
					else  //not both children of w are BLACK => case3 or case4
					{
						if (colorOf(w->right) == BLACK)  //i.e. w->left == RED => Case 3
						{
							w->setColor(RED);
							w->left->setColor(BLACK); //w->left is RED hence it cannot be NULL
							rightRot(root, w);
						}
						else //case4   w->right == RED
						{
							w->setColor(x->parent->getColor());
							x->parent->setColor(BLACK);
							w->right->setColor(BLACK);
							leftRot(root, x->parent);
							Node * p = x;
							x = root;  //we have finished
							removeSentinel(p);
						}
					}
				}
			}
			else      //w is NULL
			{
				//set x->parent BLACK, pass x up to its parent and remove the sentinel if necessary
				x->parent->setColor(BLACK);
				Node * p = x;
				x = x->parent;
				removeSentinel(p);
			}
		}
		else  //symmetric case if x is a right child
		{
			w = x->parent->left;
			if (w != NULL)
			{
				if (w->getColor() == RED) //Case 1
				{
					w->parent->setColor(RED);  //w cannot be NULL, since it is RED
					w->setColor(BLACK);
					rightRot(root, x->parent);
					w = x->parent->left;
				}
				else  //color of w is BLACK => case 2, 3 or 4
				{
					if (colorOf(w->left) == BLACK && colorOf(w->right) == BLACK)  //case 2
					{
						w->setColor(RED);
						Node * p = x;
						x = x->parent;
						x->setColor(BLACK);
						removeSentinel(p);
					}
					else  //not both children of w are BLACK => case3 or case4
					{
						if (colorOf(w->left) == BLACK)  //i.e. w->right == RED => Case 3
						{
							w->setColor(RED);
							w->right->setColor(BLACK); //w->left is RED hence it cannot be NULL
							leftRot(root, w);
						}
						else //case4   w->left == RED
						{
							w->setColor(x->parent->getColor());
							x->parent->setColor(BLACK);
							w->left->setColor(BLACK);
							rightRot(root, x->parent);
							Node * p = x;
							x = root;  //we have finished
							removeSentinel(p);
						}
					}
				}
			}
			else   //w is NULL
			{
				Node * p = x;
				x = x->parent;
				x->setColor(BLACK);
				removeSentinel(p);
			}
		}
	}
	root->setColor(BLACK);
}

Node * search(Node * root, int keyToSearch)
{
	if (root == NULL)
		return NULL;
	Node * p = root;
	while (p != NULL)  //infinite loop
	{
		int key = p->getKey();
		if (key == keyToSearch)
			return p;
		if (keyToSearch < key)
			p = p->left;
		else
			p = p->right;
	}
	return NULL;
}

void BuildRBTree(Node * & root);

/*int main(void)
{
/*Node * root = new Node(7, RED);

rbInsert(root, new Node(2));
rbInsert(root, new Node(11));
rbInsert(root, new Node(1));
rbInsert(root, new Node(5));
rbInsert(root, new Node(4));
rbInsert(root, new Node(14));
rbInsert(root, new Node(15));
rbInsert(root, new Node(8));

printTree(root, 0);
cout <<"--------------------------------------------\n";

rbDelete(root, root->left->right);

printTree(root, 0);
cout <<"--------------------------------------------\n";  */

/*Node * root = NULL;

//BuildRBTree(root);

//printTree(root, 0);

int choice = 1;
while(choice != 0)
{
cout <<" 0 = Exit\n";
cout <<" 1 = Insert\n";
cout <<" 2 = Delete\n";
cout <<" 3 = Print\n";
cout <<"You choose: ";
cin >>choice;
switch(choice)
{
case 0:
break;
case 1:
{
int key;
cout <<"Give the key of the node to insert:\n";
cin >>key;
rbInsert(root, new Node(key));
}
break;
case 2:
{
int key;
cout <<"Give the key of the node to delete:\n";
cin >>key;
Node * p = search(root, key);
rbDelete(root, p);
}
break;
case 3:
printTree(root, 0);
break;
}
}


//delete root;
//delete nil;

return 0;
}*/

void BuildRBTree(Node * & root)
{
	char c = 'y';
	while (c == 'y')
	{
		int key;
		cout << "Give the keyof the node to insert:\n";
		cin >> key;
		rbInsert(root, new Node(key));
		cout << "Insert another node? [y/n]\n";
		cin >> c;
	}
}
