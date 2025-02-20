#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include <ctime>
#include "Profiler.h"
using namespace std;

struct node
{
	node* left, * right;
	int info, size;
};

node* createNode(int info)
{
	node* newNode = new node;
	newNode->info = info;
	newNode->left = newNode->right = NULL;
	newNode->size = 1;
	return newNode;
}

node* buildOSTree(int st, int dr, Operation *assignments, Operation *comparisons)
{
	if (st > dr)
		return NULL;
	if (comparisons) comparisons->count();
	int mij,leftSize=0,rightSize=0;
	mij = (st + dr) / 2;
	if (assignments) assignments->count();
	node* newNode = createNode(mij);
	newNode->left = buildOSTree(st, mij - 1, assignments, comparisons);
	newNode->right = buildOSTree(mij + 1, dr, assignments, comparisons);
	if (newNode->left)
	{
		if (assignments) assignments->count(2);
		leftSize = newNode->left->size;
	}
	if (newNode->right)
	{
		if (assignments) assignments->count(2);
		rightSize = newNode->right->size;
	}
	if (assignments) assignments->count();
	newNode->size = 1 + leftSize + rightSize;
	return newNode;
}

node* buildOSTree(int sizeArray, Operation* assignments, Operation* comparisons)
{
	return buildOSTree(1, sizeArray, assignments, comparisons);
}

void pp(node* root, int ind)
{
	if (root != NULL)
	{
		pp(root->right, ind + 4);
		if (ind > 0)
			cout << setw(ind) << " ";
	}
	else
		return;
	cout << root->info << "(Size: " << root->size << ")" << endl;
	pp(root->left, ind + 4);
}

node* OSSelect(node* root, int ind, Operation* comparisons, Operation* assignments)
{
	if (!root)
		return NULL;
	int leftSize = 0, rank;
	if (root->left)
	{
		leftSize = root->left->size;
		if (assignments) assignments->count();
	}
	rank = leftSize + 1;
	if (comparisons)comparisons->count();
	if (ind == rank)
		return root;
	else
	{
		if (comparisons)comparisons->count();
		if (ind < rank)
			return OSSelect(root->left, ind,NULL,NULL);
		else
		{
			int newInd = ind - rank;
			return OSSelect(root->right, newInd,NULL,NULL);
		}
	}
}

node* findMinimumFromOSTree(node* root, Operation* assignments, Operation* comparisons)
{
	while (root && root->left)
	{
		root = root->left;
		if (assignments) assignments->count();
		//if (comparisons) comparisons->count();
	}
	return root;
}

void updateSizeOSTree(node* root, Operation* assignments)
{
	if (root)
	{
		int leftSize = 0, rightSize = 0;
		if (root->left)
		{
			if (assignments) assignments->count();
			leftSize = root->left->size;
		}
		if (root->right)
		{
			if (assignments) assignments->count();
			rightSize = root->right->size;
		}
		root->size = 1 + leftSize + rightSize;
		if (assignments) assignments->count();
	}
}

node* OSDelete(node* root, int info, Operation* assignments, Operation* comparisons)
{
	if (!root)
		return NULL;
	if (comparisons) comparisons->count();
	if (info < root->info)
	{
		if (assignments) assignments->count();
		root->left = OSDelete(root->left, info, assignments, comparisons);
	}
	else
	{
		if (comparisons) comparisons->count();
		if (info > root->info)
		{
			if (assignments) assignments->count();
			root->right = OSDelete(root->right, info, assignments, comparisons);
		}
		else
		{
			if (comparisons)comparisons->count();
			if (root->left == NULL && root->right == NULL)
			{
				//node is leaf
				delete root;
				return NULL;
			}
			//node has one child
			node* aux;
			if (root->left == NULL)
			{
				if (assignments) assignments -> count();
				aux = root->right;
				delete root;
				return aux;
			}
			else
			{
				if (root->right == NULL)
				{
					if (assignments) assignments->count();
					aux = root->left;
					delete root;
					return aux;
				}
			}
			//node has 2 children
			aux = findMinimumFromOSTree(root->right,NULL,NULL);
			root->info = aux->info;//copy the succesors' value
			root->right = OSDelete(root->right, aux->info,assignments,comparisons);//delete the successor
			if (assignments) assignments->count(3);
		}
		
	}
	updateSizeOSTree(root,assignments);
	return root;
}

void demoRandomOSSelect(node* root, int n, int testCount) {
	int i, k;
	cout << "\nExecute OS-SELECT\n";
	srand(time(0));
	for (i = 0; i < testCount; i++) {
		k = rand() % n + 1;
		node* found = OSSelect(root, k, NULL,NULL);
		if (found != NULL) {
			cout << "The " << k << "-th smallest element is: " << found->info << endl;
		}
		else {
			cout << "Element not found " << k << endl;
		}
	}
}

void demoOSDelete(node*& root, int n) 
{
	int i, k;
	cout << "\nExecute OS-DELETE:\n";
	srand(time(0)+1);
	for (i = 0; i < 3; i++) 
	{
		//k = rand() % n + 1; // Random index between 1 and n
		k = 6;
		node* selected = OSSelect(root, k, NULL,NULL);
		if (selected != NULL) 
		{
			cout << "Deleting the " << k << "-th smallest element: " << selected->info << endl;
			root = OSDelete(root, selected->info, NULL, NULL);
			cout << "Tree after deletion:\n";
			pp(root, 0);
			cout << endl;
		}
		else 
		{
			cout << "Element not found for index " << k << endl;
		}
	}
}

void demo()
{
	int n = 11;
	node* root = buildOSTree(n, NULL, NULL);
	cout << "Pretty print\n";
	pp(root, 0);
	demoRandomOSSelect(root, n, 4);
	demoOSDelete(root, n);

}

void performanceOSTree()
{
	int n, t;
	Profiler profiler;
	for (n = 100; n <= 1000; n = n + 100)
	{
		Operation build_assignments = profiler.createOperation("Build_Assignments", n);
		Operation build_comparisons = profiler.createOperation("Build_Comparisons", n);
		Operation select_assignments = profiler.createOperation("Select_Assignments", n);
		Operation select_comparisons = profiler.createOperation("Select_Comparisons", n);
		Operation delete_assignments = profiler.createOperation("Delete_Assignments", n);
		Operation delete_comparisons = profiler.createOperation("Delete_Comparisons", n);
		//(t = 0; t < 5; t++)
		//{
			node* root = buildOSTree(n, &build_assignments, &build_comparisons);
			srand(time(0));
			int i, k;
			for (i = 0; i < n; ++i) 
			{
				k = rand() % (n - i) + 1;
				OSSelect(root, k, &select_comparisons,&select_assignments);
				root = OSDelete(root, k, &delete_assignments, &delete_comparisons);
			}
		//}
		/*profiler.divideValues("Build_Assignments", 5);
		profiler.divideValues("Build_Comparisons", 5);
		profiler.divideValues("Select_Assignments", 5);
		profiler.divideValues("Select_Comparisons", 5);
		profiler.divideValues("Delete_Assignments", 5);
		profiler.divideValues("Delete_Comparisons", 5);*/
	}
	profiler.createGroup("Build", "Build_Assignments", "Build_Comparisons");
	profiler.createGroup("Select", "Select_Assignments", "Select_Comparisons");
	profiler.createGroup("Delete", "Delete_Assignments", "Delete_Comparisons");
	profiler.showReport();
}

int main()
{
	demo();
	performanceOSTree();
}