#include <iostream>
#include "Profiler.h"
using namespace std;

struct tree
{
	int index;
	tree* children[100];
	int childCount;
};

struct binaryTree
{
	int index;
	binaryTree *left, *right;
};

struct stackN
{
	binaryTree* node;
	stackN* next;
};

struct stack
{
	stackN* top;
};

struct binaryNode
{
	int k;
	binaryNode* child, * right;
};

void preorderRecursive(binaryTree* root, Operation* o)
{
	if (!root) 
		return;
	if (o)
		o->count();
	cout << root->index << ' ';
	preorderRecursive(root->left, o);
	preorderRecursive(root->right, o);
}

void inorderRecursive(binaryTree* root, Operation* o)
{
	if (!root) 
		return;
	inorderRecursive(root->left, o);
	if (o)
		o->count();
	cout << root->index << ' ';
	inorderRecursive(root->right, o);
}

void postorderRecursive(binaryTree* root, Operation* o)
{
	if (!root) 
		return;
	postorderRecursive(root->left, o);
	postorderRecursive(root->right, o);
	if (o)
		o->count();
	cout << root->index << ' ';
}

void initStack(stack& st)
{
	st.top = nullptr;
}

void pushStack(stack& st, binaryTree* node)
{
	stackN* newNode = new stackN;
	newNode->node = node;
	newNode->next = st.top;
	st.top = newNode;
}

binaryTree* popStack(stack& st)
{
	if (st.top == nullptr)
		return nullptr;
	stackN* aux = st.top;
	binaryTree* node = aux->node;
	st.top = st.top->next;
	delete aux;
	return node;
}

binaryTree* peekStack(const stack& st)
{
	if (st.top == nullptr)
		return nullptr;
	return st.top->node;
}

bool isEmpty(const stack& st)
{
	return (st.top == nullptr);
}

void freeStack(stack& st)
{
	while (!isEmpty(st))
		popStack(st);
}

void preorderIterative(binaryTree* root, Operation* o)
{
	if (!root) return;
	stack st;
	initStack(st);
	pushStack(st, root);
	while (!isEmpty(st))
	{
		binaryTree* node = popStack(st);
		if (o)
			o->count();
		cout << node->index << ' ';
		if (node->right)
			pushStack(st, node->right);
		if (node->left)
			pushStack(st, node->left);
	}
	freeStack(st);
}

void inorderIterative(binaryTree* root, Operation* o)
{
	if (!root) return;
	stack st;
	initStack(st);
	binaryTree* current = root;
	while (current || !isEmpty(st))
	{
		while (current)//goto the leftmost node
		{
			pushStack(st, current); // Push all left nodes
			current = current->left;
		}
		current = popStack(st); // Process current node
		if (o)
			o->count();
		cout << current->index << ' ';
		current = current->right; // Move to right subtree
	}
	freeStack(st);
}

void postorderIterative(binaryTree* root, Operation* o)
{
	if (!root) return;
	stack st;
	initStack(st);
	binaryTree* lastVisited = nullptr;
	binaryTree* current = root;
	while (current || !isEmpty(st))
	{
		if (current)
		{
			pushStack(st, current); // Traverse to the leftmost node
			current = current->left;
		}
		else
		{
			binaryTree* peekNode = peekStack(st);
			if (peekNode->right && lastVisited != peekNode->right)
			{
				current = peekNode->right; // Process right subtree
			}
			else
			{
				// Process current node
				if (o)
					o->count();
				cout << peekNode->index << ' ';
				lastVisited = popStack(st); // Mark as visited
			}
		}
	}
	freeStack(st);
}

void demoTraversals()
{
	// Create a binary tree
	binaryTree* root = new binaryTree{ 1, nullptr, nullptr };
	root->left = new binaryTree{ 2, nullptr, nullptr };
	root->right = new binaryTree{ 3, nullptr, nullptr };
	root->left->left = new binaryTree{ 4, nullptr, nullptr };
	root->left->right = new binaryTree{ 5, nullptr, nullptr };
	root->right->left = new binaryTree{ 6, nullptr, nullptr };
	root->right->right = new binaryTree{ 7, nullptr, nullptr };

	cout << "\nPreorder Recursive: ";
	preorderRecursive(root, nullptr);
	cout << "\nPreorder Iterative: ";
	preorderIterative(root, nullptr);
	cout << "\n\nInorder Recursive: ";
	inorderRecursive(root, nullptr);
	cout << "\nInorder Iterative: ";
	inorderIterative(root, nullptr);
	cout << "\n\nPostorder Recursive: ";
	postorderRecursive(root, nullptr);
	cout << "\nPostorder Iterative: ";
	postorderIterative(root, nullptr);
}

void performance(Profiler& profiler) 
{
	int size, i;
	for (size = 100; size <= 10000; size = size + 100) 
	{
		int* vect = new int[size];
		FillRandomArray(vect, size, 10, 50000, true);
		binaryTree* root = new binaryTree{ vect[0], nullptr, nullptr };
		for (i = 1; i < size; i++) {
			binaryTree* current = root;
			while (true) {
				if (vect[i] < current->index) {
					if (!current->left) {
						current->left = new binaryTree{ vect[i], nullptr, nullptr };
						break;
					}
					current = current->left;
				}
				else {
					if (!current->right) {
						current->right = new binaryTree{ vect[i], nullptr, nullptr };
						break;
					}
					current = current->right;
				}
			}
		}
		Operation preorderRecursiveOps = profiler.createOperation("Preorder_Recursive", size);
		Operation preorderIterativeOps = profiler.createOperation("Preorder_Iterative", size);
		preorderRecursive(root, &preorderRecursiveOps);
		preorderIterative(root, &preorderIterativeOps);
		Operation inorderRecursiveOps = profiler.createOperation("Inorder_Recursive", size);
		Operation inorderIterativeOps = profiler.createOperation("Inorder_Iterative", size);
		inorderRecursive(root, &inorderRecursiveOps);
		inorderIterative(root, &inorderIterativeOps);
		Operation postorderRecursiveOps = profiler.createOperation("Postorder_Recursive", size);
		Operation postorderIterativeOps = profiler.createOperation("Postorder_Iterative", size);
		postorderRecursive(root, &postorderRecursiveOps);
		postorderIterative(root, &postorderIterativeOps);
	}
	profiler.createGroup("Preorder_Comparisons", "Preorder_Recursive", "Preorder_Iterative");
	profiler.createGroup("Inorder_Comparisons", "Inorder_Recursive", "Inorder_Iterative");
	profiler.createGroup("Postorder_Comparisons", "Postorder_Recursive", "Postorder_Iterative");
}

void prettyPrintVectorTati(int t[], int n)
{
	int i, levels[100], current, level, j;
	for (i = 0; i < n; i++)
	{
		current = i;
		level = 0;
		while (t[current] != -1)
		{
			current = t[current] - 1;
			level++;
		}
		levels[i] = level;
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < levels[i]; j++)
		{
			cout << " ";
		}
		cout << i + 1 << endl;
	}
}

tree* Transformation1(int tati[], int n)
{
	tree* nodes[100];
	tree* root = nullptr;
	int i;
	for (i = 0; i < n; i++)//create nodes
		nodes[i] = new tree{ i + 1,{},0 };
	for (i = 0; i < n; i++)
	{
		if (tati[i] == -1)
		{
			root = nodes[i];
		}
		else
		{
			tree* parent = nodes[tati[i] - 1];
			parent->children[parent->childCount++] = nodes[i];
		}
	}
	return root;
}

void prettyPrintAfterTransformation1(tree* root, int level)
{
	if (!root)
		return;
	int i;
	for (i = 0; i < level; i++)
		cout << ' ';
	cout << root->index << endl;
	for (i = 0; i < root->childCount; i++)
		prettyPrintAfterTransformation1(root->children[i], level + 1);
}

binaryNode* Transformation2(tree* root)
{
	int i;
	if (!root)
		return nullptr;
	binaryNode* bRoot = new binaryNode{ root->index, nullptr, nullptr };
	binaryNode* previousSibling = nullptr;
	for (i = 0; i < root->childCount; i++)
	{
		binaryNode* child = Transformation2(root->children[i]);
		if (i == 0)
			bRoot->child = child;
		else
			previousSibling->right = child;
		previousSibling = child;
	}
	return bRoot;
}

void prettyPrintAfterTransformation3(binaryNode* root, int level)
{
	if (!root) 
		return;
	int i;
	for (i = 0; i < level; i++)
		cout << ' ';
	cout << root->k << endl;
	prettyPrintAfterTransformation3(root->child, level + 1);
	prettyPrintAfterTransformation3(root->right, level);
}

void demoMultiwayTrees()
{
	int tati[] = { 2, 7, 5, 2, 7, 5, -1, 2 };
	int n = 8;
	cout << endl <<  "Pretty Printed Multiway Tree from Parent Array:" << endl;
	prettyPrintVectorTati(tati, n);
	cout << endl << "Pretty Printed Multiway Tree after First Transformation:" << endl;
	tree* root = Transformation1(tati, n);
	prettyPrintAfterTransformation1(root, 0);
	cout << endl << "Pretty Printed Multiway Tree after Second Transformation:" << endl;
	binaryNode* root2 = Transformation2(root);
	prettyPrintAfterTransformation3(root2, 0);
}

int main()
{
	Profiler profiler;
	demoTraversals();
	//performance(profiler);
	//profiler.showReport();
	demoMultiwayTrees();
	return 0;
}
	