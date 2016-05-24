// File:        redblacktree.cpp
// Author:      Meihui Yan & Jiameng Duan
// Date:        2016-03-12
// Description: Contains implementation of some RedBlackTree functions.
//              This file does not require modification.

#ifdef _REDBLACKTREE_H_
#include "minmax.h"
#include <iostream>

// recursive helper function for deep copy
// creates a new node based on sourcenode's contents, links back to parentnode,
// and recurses to create left and right children
template <class T>
Node<T>* RedBlackTree<T>::CopyTree(Node<T>* sourcenode, Node<T>* parentnode)
{
	if ((sourcenode != nullptr) && (parentnode == nullptr))
	{
		root = new Node<T>(sourcenode->data);
		root->is_black = sourcenode->is_black ;
		CopyTree(sourcenode->left, root);
		CopyTree(sourcenode->right, root);
		return root;
	}
	Node<T>* New = nullptr;
	if ((sourcenode != nullptr) && (parentnode != nullptr))
	{
		New = this->BSTInsert(sourcenode->data);
		New->is_black = sourcenode->is_black;
		New->p = parentnode;
		CopyTree(sourcenode->left, New);
		CopyTree(sourcenode->right, New);
		return New;
	}
	else
		return nullptr;
}


// recursive helper function for tree deletion
// deallocates nodes in post-order
template <class T>
void RedBlackTree <T>::RemoveAll(Node<T>* node)
{
	//std::cout << " out here" << endl;
	//if (node == nullptr)
	//{
	//	//cout << "null pointer" << endl;
	//	return;
	//}
	//else
	if (node != nullptr)
	{
		RemoveAll(node->left);
		RemoveAll(node->right);
		delete node;
		return;
	}
}

// Tree fix, performed after removal of a black node
// Note that the parameter x may be NULL
template <class T>
void RedBlackTree <T> ::RBDeleteFixUp(Node<T>* x, Node<T>* xparent, bool xisleftchild)
{
	Node<T>* y;
	while (x != nullptr && x != root && x->is_black)
	{
		if (xisleftchild) // x is left child
		{
			y = xparent->right; // x’s sibling
			if (!(y->is_black))
				y->is_black = true;
			xparent->is_black = false; // p was black
			LeftRotate(xparent);
			y = xparent->right;
			if (y->left->is_black && y->right->is_black)
			{
				y->is_black = false;
				x = xparent; //and into while again ...
				xparent = x->p;
			}
			else
			{
				if (y->is_black)
				{
					y->left->is_black = true;
					y->is_black = false;
					RightRotate(y);
					y = xparent->right;
				}
				y->is_black = xparent->is_black;
				xparent->is_black = true;
				y->right->is_black = true;
				LeftRotate(x->p);
				x = root;
			}
		}
		else
		{
			y = xparent->left; // x’s sibling
			if (y->is_black == false)
				y->is_black = true;
			xparent->is_black = false; 
			RightRotate(xparent);
			y = xparent->left;
			if (y->left->is_black && y->right->is_black)
			{
				y->is_black = false;
				x = xparent; //and into while again ...
				xparent = x->p;
			}
			else
			{
				if (y->is_black)
				{
					y->right->is_black = true;
					y->is_black = false;
					LeftRotate(y);
					y = x->p->left;
				}
				y->is_black = xparent->is_black;
				xparent->is_black = true;
				y->left->is_black = true;
				RightRotate(xparent);
				x = root;
			}
		}
	}
}

// Calculates the height of the tree
// Requires a traversal of the tree, O(n)
template <class T>
int RedBlackTree<T> ::CalculateHeight(Node<T>* node) const
{
	if (node == nullptr)  //height of a empty tree equals zero
		return 0;
	else
	{
		return (1 + max(CalculateHeight(node->left), CalculateHeight(node->right)));
	}
}

// default constructor
template <class T>
RedBlackTree<T> :: RedBlackTree()
{
	root = nullptr;
	size = 0;
}

// copy constructor, performs deep copy of parameter
template <class T>
RedBlackTree<T> :: RedBlackTree(const RedBlackTree<T>& rbtree)
{
	this->size = rbtree.size;
	root = this->CopyTree(rbtree.root, nullptr);
}

// destructor
// Must deallocate memory associated with all nodes in tree.
template <class T>
RedBlackTree<T> :: ~RedBlackTree()
{
	RemoveAll();
	size = 0;
}

// overloaded assignment operator
template <class T>
RedBlackTree<T>& RedBlackTree<T> :: operator=(const RedBlackTree<T>& rbtree)
{
	if (this == &rbtree)
		return *this;
	else
	{
		RemoveAll();
		root = CopyTree(rbtree.root, nullptr);
		size = rbtree.size;
		return *this;
	}
}

// Calls BSTInsert and then performs any necessary tree fixing.
// If item already exists, do not insert and return false.
// Otherwise, insert, increment size, and return true.
template <class T>
bool RedBlackTree<T> :: Insert(T item)
{
	if (this->Search(item)) //return flase if the item already exists.
		return false;

	//insert this item and determine the color of this node
	else  //item does not exist

	{
		Node<T>* New = this->BSTInsert(item); //calling BSTInsert
		size++;  //increment size
		//Tree fixing up
		Node<T>* temp;
		New->is_black = false; //coulour the new node as red
		while ((New != root) && (New->p->is_black == false))
		{
			if (New->p == New->p->p->left)
			{
				temp = New->p->p->right;   //uncle of insertItem
				if ((temp!=nullptr) && (temp->is_black == false))
				{
					New->p->is_black = true;
					temp->is_black = true;
					New->p->p->is_black = false;
					New = New->p->p;
				}
				else
				{
					if (New == New->p->right)
					{
						New = New->p;
						LeftRotate(New);
					}
					New->p->is_black = true;
					New->p->p->is_black = false;
					RightRotate(New->p->p);
				}
			}
			else
			{
				temp = New->p->p->left;   //uncle of insertItem
				if ((temp != nullptr) && (temp->is_black == false))
				{
					New->p->is_black = true;
					temp->is_black = true;
					New->p->p->is_black = false;
					New = New->p->p;
				}
				else
				{
					if (New == New->p->left)
					{
						New = New->p;
						RightRotate(New);
					}
					New->p->is_black = true;
					New->p->p->is_black = false;
					LeftRotate(New->p->p);
				}
			}
		}
		root->is_black = true; //make sure root is black
		return true;
	}
}

// Removal of an item from the tree.
// Must deallocate deleted node after RBDeleteFixUp returns
template <class T>
bool RedBlackTree<T> ::Remove(T item)
{
	if (Search(item)) //item exists
	{
		size--;//decrement size

		//loacte the item, and pointed by z
		Node<T>* node = root;
		Node<T>* z = nullptr;
		Node<T>* x;
		Node<T>* xp;
		Node<T>* y;
		while (node != NULL)
		{
			if (item == node->data)
			{
				z = node;
				break;
			}
			else if (item < node->data)
				node = node->left;
			else
				node = node->right;
		}

		bool Left = false;
		if (z->left == nullptr || z->right == nullptr)
			y = z; //node to be removed
		else
			y = Predecessor(z);
		if (y->left != nullptr)
		{
			x = y->left;
			xp = y;
			Left = true;
		}
		else
		{
			x = y->right;
			xp = y;
			Left = false;
		}
		if (x != nullptr)
			xp = y->p; //detach x from y (if x is not null)
		if (y->p == nullptr) //y is the root
		{
			root = x;
			xp = nullptr;
			Left = false;
		}
		else
		{
			// Attach x to y’s parent
			if (y == y->p->left) //left child
			{
				y->p->left = x;
				xp = y->p;
				Left = true;
			}
			else
			{
				y->p->right = x;
				xp = y->p;
				Left = false;
			}
		}
		if (y != z) //i.e. y has been moved up
			z->data = y->data; //replace z with y
		if (y->is_black)
			RBDeleteFixUp(x, xp, Left);//x could be null 

		return true;
	}

	else
		return false;
}


// deletes all nodes in the tree. Calls recursive helper function.
template <class T>
void RedBlackTree<T>::RemoveAll()
{
	RemoveAll(root);
	root = nullptr;
	size = 0;
}

// returns the number of items in the tree
template <class T>
int RedBlackTree<T>::Size() const
{
	return size;
}

// returns the height of the tree, from root to deepest null child. Calls recursive helper function.
// Note that an empty tree should have a height of 0, and a tree with only one node will have a height of 1.
template <class T>
int RedBlackTree<T>::Height() const
{
	return CalculateHeight(this->root);
}

#endif