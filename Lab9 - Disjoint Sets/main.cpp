#include <iostream>
#include "Profiler.h"
using namespace std;

struct node
{
	int parent, rank;
};

struct edge
{
	int head, tail, weight;
};

node makeSet(int x, Operation* op)
{
	node newSet;
	newSet.parent = x;
	newSet.rank = 0;
	if (op)
		op->count(2);
	return newSet;
}

//use path compression
int findRepresentative(node* set, int x, Operation* op)
{
	if (op) op->count();
	if (set[x].parent != x)
	{
		if (op) op->count();
		set[x].parent = findRepresentative(set, set[x].parent, op);
	}
	return set[x].parent;
}

void unionSet(node* set, int x, int y, Operation* op)
{
	int findX = findRepresentative(set, x, op), findY = findRepresentative(set, y, op);
	if (op) op->count();
	if (findX != findY)
	{
		if (op) op->count(2); 
		if (set[findX].rank < set[findY].rank)
			set[findX].parent = findY;
		else
		{
			if (op) op->count();
			if(set[findX].rank > set[findY].rank)
				set[findY].parent = findX;
			else
			{
				set[findY].parent = findX;
				set[findX].rank++;
				if (op) op->count();
			}
		}
	}
}

void displaySets(node* set, int n)
{
	cout << "Sets: " << endl;
	int i;
	for (i = 0; i < n; i++)
		cout << "Element " << i << ", parent: " << set[i].parent << " , rank: " << set[i].rank << endl;
	cout << endl;
}

void sortEdges(edge* edges, int n, Operation* op)
{
	int i, j;
	for (i = 0; i < n-1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (op) op->count();
			if (edges[i].weight > edges[j].weight)
			{
				edge aux = edges[j];
				edges[j] = edges[i];
				edges[i] = aux;
				if (op) op->count(3);
			}
		}
	}
}

void displayEdges(edge* edges, int n) 
{
	int i;
	cout << "Graph Edges (head -- tail == weight):" << endl;
	for (i = 0; i < n; i++) 
	{
		cout << edges[i].head << " -- " << edges[i].tail << " == " << edges[i].weight << endl;
	}
	cout << endl;
}

void displayMST(edge* mst, int size)
{
	int i;
	cout << "Minimum spanning tree edges: " << endl;
	for (i = 0; i < size; i++)
	{
		cout << mst[i].head << " -- " << mst[i].tail << " == " << mst[i].weight << endl;
	}
}

void kruskalMST(int nodesN, int edgesN, edge* edges, Operation* opM, Operation* opU, Operation* opF)
{
	sortEdges(edges, edgesN,NULL);
	node* set = new node[nodesN];
	edge* mst = new edge[nodesN - 1];
	int i,mstN=0,rootT,rootH;
	for (i = 0; i < nodesN; i++)
	{
		set[i] = makeSet(i, opM);
		if (opM) opM->count();
	}
	for (i = 0; i < edgesN && mstN < nodesN - 1; i++)
	{
		rootH = findRepresentative(set, edges[i].head, opF);
		rootT = findRepresentative(set, edges[i].tail, opF);
		if (opF) opF->count(3);
		if (rootH != rootT)
		{
			mst[mstN++] = edges[i];
			if (opF) opF->count();
			unionSet(set, edges[i].head, edges[i].tail,opU);
		}
	}
	displayMST(mst, mstN);
	// Cleanup
	delete[] set;
	delete[] mst;
}

void demoKruskal()
{
	const int nodesN = 5, edgesN = 9;
	edge edges[edgesN] = { {0, 1, 2}, {0, 3, 6}, {1, 3, 4}, {1, 4, 5}, {1, 2, 3}, {2, 4, 7}, {3, 4, 9}, {0, 2, 4}, {2, 3, 1} };
	displayEdges(edges, edgesN);
	kruskalMST(nodesN, edgesN, edges,NULL,NULL,NULL);
}

void demo()
{
	int n = 10, i;
	node* set = new node[n];
	for (i = 0; i < n; i++)
	{
		set[i] = makeSet(i,NULL);
	}
	cout << "Initial sets" << endl;
	displaySets(set, n);
	unionSet(set, 0, 1,NULL);
	unionSet(set, 2, 3,NULL);
	unionSet(set, 4, 5,NULL);
	unionSet(set, 6, 7,NULL);
	unionSet(set, 1, 3,NULL);
	cout << "After joins" << endl;
	displaySets(set, n);
	for(i=0; i<5; i++)
		cout<< "FIND_SET(" << i << ") = " << findRepresentative(set,i,NULL) << endl;
	cout << "final sets" << endl;
	displaySets(set, n);
	delete[] set;

	demoKruskal();
}

void generateRandomGraph(edge* edges, int n, int m)
{
	srand(time(0));
	int edgeN = 0, i, u, v,j;
	bool** adjacencyMatrix = new bool* [n];
	for (i = 0; i < n; i++)
	{
		//initialization
		adjacencyMatrix[i] = new bool[n];
		for (j = 0; j < n; j++)
			adjacencyMatrix[i][j] = false;
	}
	for (i = 0; i < n - 1; i++)
	{
		//to make sure the graph is connected
		edges[edgeN++] = { i,i + 1,rand() % 100 + 1 };
		adjacencyMatrix[i][i + 1]= adjacencyMatrix[i+1][i] = true;
	}
	while (edgeN < m)
	{
		u = rand() % n;
		v = rand() % n;
		if (u != v && (!adjacencyMatrix[u][v]))
		{
			edges[edgeN++] = { u,v,rand() % 100 + 1 };
			adjacencyMatrix[u][v] = adjacencyMatrix[v][u] = true;
		}
	}
	//cleanup
	for (i = 0; i < n; i++)
		delete[] adjacencyMatrix[i];
	delete[] adjacencyMatrix;
}

void performance()
{
	Profiler profiler;
	int n, m, t;
	const int averageruntime = 5;
	for (t = 0; t < averageruntime; t++)
	{
		for (n = 100; n < 10000; n = n + 100)
		{
			m = 4 * n;
			Operation makeOps = profiler.createOperation("MAKE", n);
			Operation unionOps = profiler.createOperation("UNION", n);
			Operation findOps = profiler.createOperation("FIND", n);
			//for (t = 0; t < averageruntime; t++)
			//{
			edge* edges = new edge[m];
			generateRandomGraph(edges, n, m);
			kruskalMST(n, m, edges, &makeOps, &unionOps, &findOps);
			//cleanup
			delete[] edges;
			//}
			/*profiler.divideValues("MAKE", averageruntime);
			profiler.divideValues("UNION", averageruntime);
			profiler.divideValues("FIND", averageruntime);*/

		}
	}
	profiler.divideValues("MAKE", averageruntime);
	profiler.divideValues("UNION", averageruntime);
	profiler.divideValues("FIND", averageruntime);
	profiler.createGroup("KRUSKAL_DisjointSetOperations", "MAKE", "UNION", "FIND");
	profiler.showReport();
}

int main()
{
	demo();
	//performance();
	return 0;
}