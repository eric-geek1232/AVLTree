/*By Eric 11/01/2021 -AVL Tree-*/
#include <iostream>

using namespace std;

struct Node
{
	int data;
	int FE;
	Node *left;
	Node *right;
	Node *parent;
};

class AVLTree
{
	private:
		Node *root;
		
		int getFE(Node *node);
		Node *createNode(int number);
		Node *leftRotation(Node *node);
		Node *rightRotation(Node *node);
		Node *doubleRotation_left(Node *node);
		Node *doubleRotation_right(Node *node);
		Node *insertAVL(Node *newNode, Node *tree);
		Node *removeAVL(Node *tree, int number);
		void searchNodeAVL(Node *tree, int number, int count);
		void printAVLTree(Node *tree, int count);
		void preOrderAVL(Node *tree);
		void inOrderAVL(Node *tree);
		void postOrderAVL(Node *tree);
		Node *getPredecessor(Node *node);	
		Node *getSuccessor(Node *node);
		Node *getMinNode(Node *tree);
		Node *getMaxNode(Node *tree);
		int getDepthAVL(Node *tree);
		int getSizeAVL(Node *tree);
		
	public:
		AVLTree();
		void insert(int number);
		void remove(int number);
		void search(int number);
		void show();
		void preOrder();
		void inOrder();
		void postOrder();
		int minValue();
		int maxValue();
		int depth();
		int size();
		bool isEmpty();
};

AVLTree::AVLTree()
{
	root = NULL;
}

int AVLTree::getFE(Node *node)
{
	if (node==NULL) {
		return 0;	
	} else {
		return node->FE;	
	}
}

Node *AVLTree::leftRotation(Node *node)
{
	Node *node_aux = node->right;											// step 1: get the right side subtree;
	
	node_aux->parent = node->parent;										// step 2: change parents
    node->parent = node_aux;
    
	node->right = node_aux->left;											// step 3: change nodes
    node_aux->left = node;
    
    node->FE = max(getFE(node->left), getFE(node->right)) +1;				// step 4: get max FE + 1
    node_aux->FE = max(getFE(node_aux->left), getFE(node_aux->right)) +1;
	
	return node_aux;
}

Node *AVLTree::rightRotation(Node *node)
{
	Node *node_aux = node->left;					//   example: 4   node = 3  step 1: node_aux = 2  step 2:  node_aux->parent = 4
													//           /          /                     /            node->parent = 2
	node_aux->parent = node->parent;				//          3          2                     1
	node->parent = node_aux;						//         /          /
													//        2	         1      step 3: node_aux = 2
    node->left = node_aux->right;					//       /                                    / \.
    node_aux->right = node;							//      1                                    1   3
    
    node->FE = max(getFE(node->left), getFE(node->right)) +1;
    node_aux->FE = max(getFE(node_aux->left), getFE(node_aux->right)) +1;
    
    return node_aux;
}

Node *AVLTree::doubleRotation_left(Node *node)
{
	Node *node_aux;
	
	node->right = rightRotation(node->right);			// overwrite subTree on the right side with right rotation
	node_aux = leftRotation(node);						// rotate the whole node with left rotation 
	
	return node_aux;
}

Node *AVLTree::doubleRotation_right(Node *node)
{
	Node *node_aux;
	
	node->left = leftRotation(node->left);
	node_aux = rightRotation(node);
	
	return node_aux;
}

Node *AVLTree::createNode(int number)
{
	Node *newNode = new Node();
	
	newNode->data = number;
	newNode->FE = 1;			// default value
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
}

Node *AVLTree::insertAVL(Node *newNode, Node *tree) 
{
	if (tree == NULL) {										// insert into empty node
		cout<<"\n\n\t"<< newNode->data<< " Added\n"<< endl;
        return newNode;
    }
    
    if (newNode->data < tree->data) {						// keep iterating on the left side
    	newNode->parent = tree;								// set parent
    	tree->left = insertAVL(newNode, tree->left);	
	} else if (newNode->data > tree->data) {				// keep iterating on the right side
		newNode->parent = tree;
    	tree->right = insertAVL(newNode, tree->right);
	} else {
    	cout<< "\n\n\tDuplicate item\n"<< endl;
	}
	
    tree->FE = max(getFE(tree->left), getFE(tree->right)) +1;			// recalculate FE of parent 
    int balance = getFE(tree->left) - getFE(tree->right);				// get balance
    
    // balance
    if (balance > 1) {
        if (newNode->data < tree->left->data) {
            return rightRotation(tree);
        } else {
            return doubleRotation_right(tree);
        }
    } else if (balance < -1) {
        if (newNode->data > tree->right->data) {
            return leftRotation(tree);
        } else {
            return doubleRotation_left(tree);
        }
    }
    
    return tree;
}

void AVLTree::insert(int value)
{
	Node *newNode = createNode(value);

	root = insertAVL(newNode, root);
}

Node *AVLTree::removeAVL(Node *tree, int value)
{
    if (tree == NULL) {									// tree end and element no found
    	return NULL;	
	} 
	
    if (value < tree->data) {							// keep iterating on the the left side
        tree->left = removeAVL(tree->left, value);
    } else if (value > tree->data) {					// keep iterating on the the left side
        tree->right = removeAVL(tree->right, value);
    } else {
        Node *right_aux = tree->right;
        
    	if (tree->right == NULL) {
        	Node *left_aux = tree->left;
        	delete(tree);
        	tree = left_aux;
    	} else if (tree->left == NULL) {
            delete(tree);
            tree = right_aux;
        } else {
        	
            while(right_aux->left != NULL) {
            	right_aux = right_aux->left;						// in these cases it's when the node have 2 children
			}
			
            tree->data = right_aux->data;							// overwrite data
            tree->right = removeAVL(tree->right, right_aux->data);  // overwrite this subTree find successor
        } 
    }
    
    if (tree == NULL) { 											// successor found
		return tree;
	}
	 
    tree->FE = max(getFE(tree->left), getFE(tree->right)) +1;		// recalculate FE of parent 
    int balance = getFE(tree->left) - getFE(tree->right);			// get balance
    
    // balance
    if (balance > 1) {
        if (value > tree->left->data) {
            return rightRotation(tree);
        } else {
            return doubleRotation_right(tree);
        }
    } else if (balance < -1) {
        if (value < tree->right->data) {
            return leftRotation(tree);
        } else {
            return doubleRotation_left(tree);
        }
    }
    
    return tree;
}

void AVLTree::remove(int value) 
{
	root = removeAVL(root, value);
	
	if (root == NULL) {
		cout<< "\n\n Node: "<< value<< " not found\n"<< endl;
	} else {
		cout<< "\n\n Node: "<< value<< " found\n"<< endl;
	}
}

void AVLTree::searchNodeAVL(Node *tree, int value, int count)
{	
	if (tree == NULL) {		// tree end
		cout<< "\n\n Node: "<< value<< " not found\n"<< endl;
		return;
	} else if (tree->data == value) {
		cout<< "\n\n Node: "<< tree->data<< " found\n"<< endl;
		cout<< "\n Height: "<< getFE(tree)<< "\n"<< endl;
		cout<< "\n Level: "<< count<< "\n"<< endl;
		
		Node *predecessor = getPredecessor(tree);
		Node *successor = getSuccessor(tree);
		
		if (predecessor != NULL) {
			cout<< "\n Predecessor: "<< predecessor->data<< endl;
		} else {
			cout<< "\n Predecessor: you do not have enough nodes yet"<< endl;
		}
		
		if (successor != NULL) {
			cout<< "\n Successor: "<< successor->data<< endl;	
		} else {
			cout<< "\n Successor: you do not have enough nodes yet"<< endl; 
		}
		if (root == tree){
			cout<< "\n Kind of Node: Root"<< endl;
			if (tree->left != NULL && tree->right != NULL) {
				cout<< "\n Grade: 2\n"<< endl;
			} else if (tree->left != NULL || tree->right != NULL) {
				cout<< "\n Grade: 1\n"<< endl;
			} else {
				cout<< "\n Grade: 0\n"<< endl;
			}
		} else if (tree->left != NULL && tree->right != NULL) {
			cout<< "\n Kind of Node: Parent"<< endl;
			cout<< "\n Grade: 2\n"<< endl;
		} else if (tree->left != NULL || tree->right != NULL) {
			cout<< "\n Kind of Node: Parent"<< endl;
			cout<< "\n Grade: 1\n"<< endl;
		} else {
			cout<< "\n Kind of Node: Hoja"<< endl;	
			cout<< "\n Grade: 0\n"<< endl;
		}
	} else if (value < tree->data) { 				// keep searching on the left side
		searchNodeAVL(tree->left, value, count+1);
	} else {
		searchNodeAVL(tree->right, value, count+1); // keep searching on the right side
	}
}

void AVLTree::search(int value)
{
	searchNodeAVL(root, value, 0);
}

Node *AVLTree::getMinNode(Node *tree)
{
	if (tree == NULL) { 				// tree end
		return NULL;
	} 
	
	if (tree->left != NULL) {
		return getMinNode(tree->left);	// search on the left side
	} else {
		return tree; 					// min node found
	}
}

int AVLTree::minValue()
{
	return getMinNode(root)->data;
}

Node *AVLTree::getMaxNode(Node *tree)
{
	if (tree == NULL) { 				// tree end
		return NULL;
	} 
	
	if (tree->right != NULL) { 
		return getMaxNode(tree->right);	// search on the right side
	} else {
		return tree; 					// max node found
	}
}

int AVLTree::maxValue()
{
	return getMaxNode(root)->data;
}

Node *AVLTree::getPredecessor(Node *node)
{
	if (node == NULL) {							// end tree
		return NULL;
	} 
	
	if (node->left != NULL) {
		return getMaxNode(node->left);			// search on the left side
	}
	
	Node *aux = node->parent;
	
	while (aux != NULL && node == aux->left) {	// search predecessor
		node = aux;
		aux = aux->parent; 						// update parent
	}
	
	return aux;
}

Node *AVLTree::getSuccessor(Node *node)
{
	if (node == NULL) { 							// end tree
		return NULL;
	} 
	
	if (node->right != NULL) {
		return getMinNode(node->right); 			// search on the right side
	}
	
	Node *aux = node->parent;
	
	while (aux != NULL && node == aux->right) { 	// search sucessor
		node = aux;
		aux = aux->parent; 							// update parent
	}
	
	return aux;
}

int AVLTree::getDepthAVL(Node *tree)
{
	if (tree == NULL) {
		return 0;
	} else {
		return max(getDepthAVL(tree->left), getDepthAVL(tree->right)) +1;
	}
}

int AVLTree::depth()
{
	return getDepthAVL(root);
}

int AVLTree::getSizeAVL(Node *tree)
{
	if (tree == NULL) {
		return 0;
	} else {
		return getSizeAVL(tree->left) + getSizeAVL(tree->right) +1;
	}
}

int AVLTree::size()
{
	return getSizeAVL(root);
}

void AVLTree::preOrderAVL(Node *tree)
{
	if (tree == NULL) { 				// tree end
		return;
	} else { 
		cout<< tree->data << " - "; 	// print node
		preOrderAVL(tree->left); 		// go to the left on each node
		preOrderAVL(tree->right); 		// go to the right on each node
	}
}

void AVLTree::preOrder()
{
	preOrderAVL(root);
}

void AVLTree::inOrderAVL(Node *tree)
{
	if (tree == NULL) { 				// tree end
		return;
	} else {
		inOrderAVL(tree->left); 		// go to the left on each node
		cout<< tree->data << " - "; 	// print node
		inOrderAVL(tree->right); 		// go to the right on each node
	}
}

void AVLTree::inOrder()
{
	inOrderAVL(root);
}

void AVLTree::postOrderAVL(Node *tree)
{
	if (tree == NULL) { 				// tree end
		return;
	} else {
		postOrderAVL(tree->left); 		// go to the left on each node
		postOrderAVL(tree->right); 		// go to the right on each node
		cout<< tree->data << " - "; 	// print node
	}
}

void AVLTree::postOrder()
{
	postOrderAVL(root);
}

void AVLTree::printAVLTree(Node *tree, int count)
{
	if (tree == NULL) {							// tree end
		return;
	} else { 									// if you have more than 1 nodes in your tree
		printAVLTree(tree->right, count+1); 	// go to the right on each node
		for (int i = 0; i < count; i++) {
			cout<< "   ";						// print spaces this is stetic
		}
		cout<< tree->data<< endl; 				// print node
		printAVLTree(tree->left, count+1); 		// go to the left on each node
	}
}

void AVLTree::show()
{
	printAVLTree(root, 0);
}

bool AVLTree::isEmpty()
{
	return root == NULL ? true: false;
}


int main()
{
	AVLTree *tree = new AVLTree();
	Node *node_aux;
	int option;
	int number;
	
	do
	{
		cout<< "\t AVL Trees\n"<< endl;
		cout<< "\t.::MENU::.\n"<< endl;
		cout<< " [1] Insert node"<< endl;
		cout<< " [2] Show Tree"<< endl;
		cout<< " [3] Search node"<< endl;
		cout<< " [4] pre-order list"<< endl;
		cout<< " [5] in-order list"<< endl;
		cout<< " [6] post-order list "<< endl;
		cout<< " [7] delete node"<< endl;
		cout<< " [8] show max-value"<< endl;
		cout<< " [9] show min-value"<< endl;
		cout<< " [10] quit"<< endl;
		cout<< "\n Choose: ";
		cin>>option;
		
		switch(option) {
			case 1:
				cout<< "\n\n Type your number: ";
				cin>> number;
				
				tree->insert(number);
				break;
				
			case 2:
				if (!tree->isEmpty()) {
					cout<< "\n\n\t Showing tree...\n\n"<< endl;
				
					tree->show();
					
					cout<< "\n\n Depth: "<< tree->depth()<< "\n"<< endl;	
					cout<< " Weight: "<< tree->size()<< "\n"<< endl;
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
				break;
				
			case 3:
				if (!tree->isEmpty()) {
					cout<< "\n\n Type one number: ";
					cin>> number;
					
					cout<< "\n\n\t Verifying...\n"<< endl;
					
					tree->show();
					
					tree->search(number);	
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
				break;
				
			case 4:
				if (!tree->isEmpty()) {
					cout<< "\n\n\tVerify...\n"<< endl;
					tree->show();
					
					cout<< "\n\n Pre-order list:  ";
					tree->preOrder();
					cout<< "\n"<< endl;	
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
					break;
				
			case 5:
				if (!tree->isEmpty()) {
					cout<< "\n\n\tVerify...\n"<< endl;
					tree->show();
					
					cout<< "\n\n In-order list:  ";
					tree->inOrder();
					cout<< "\n"<< endl;
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
					
				break;
				
			case 6: 
				if (!tree->isEmpty()) {
					cout<< "\n\n\tVerify...\n"<< endl;
					tree->show();
					
					cout<< "\n\n Post-order list:  ";
					tree->postOrder();
					cout<< "\n"<< endl;	
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
				break;
				
			case 7:
				if (!tree->isEmpty()) {
					cout<< "\n\n Type your number to delete: ";
					cin>> number;
					
					system("cls");
					cout<< "\tVerify...\n"<< endl;
					tree->show();
					
					tree->remove(number);
					
					cout<< "\n\n\tVerify Again...\n"<< endl;
					tree->show();
					cout<< "\n\n"<< endl;
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
					
				break;
				
			case 8:
				if (!tree->isEmpty()) {
					cout<< "\n\n\tVerify...\n"<< endl;
					tree->show();
					
					cout<< "\n\n\n Max-value: "<< tree->maxValue()<< "\n"<< endl;		
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				}
				break;
				
			case 9:
				if (!tree->isEmpty()) {
					cout<< "\n\n\tVerify...\n"<< endl;
					tree->show();
					
					cout<< "\n\n\n Min-value: "<< tree->minValue()<< "\n"<< endl;	
				} else {
					cout<< "\n\n\t**Empty**\n\n"<< endl;
				} 
				break;
				
			case 10:
				cout<< "\n\n\t**Big Thanks for using this software**\n"<< endl;
				break;
				
			default:
				cout<< "\n\n\tWrong option... try again!\n"<< endl;
		}
		
		system("pause");
		system("cls");
	} while (option != 10);
	
	return 0;
}
