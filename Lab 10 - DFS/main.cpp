#include <iostream>
#include <cstring>
#include <algorithm>
#include "Profiler.h"
#include "catch2.hpp"


using namespace std;

struct Node
{
	int vertex;
	Node* next;
};

Node* adjL[300];
int visited[300], n, parent[300];

void initializeGraph()
{
	int i;
	for (i = 0; i < 300; i++)
	{
		adjL[i] = NULL;
		visited[i] = 0;
	}
}

void addEdge(int v1, int v2)
{
	/*if (v1 >= 200 || v2 >= 200 || v1 < 0 || v2 < 0)
	{
		cout << "Invalid edge: " << v1 << " -> " << v2 << endl;
		return;
	}*/
	Node* newNode = new Node;
	newNode->vertex = v2;
	newNode->next = adjL[v1];
	adjL[v1] = newNode;
}

void printG()
{
	int i;
	for (i = 0; i < n; i++)
	{
		cout << i << ": ";
		Node* aux = adjL[i];
		while (aux)
		{
			cout << aux->vertex << " ";
			aux = aux->next;
		}
		cout << endl;
	}
}

void typeOfEdge(int u, int v)
{
	if (visited[v] == 0)
	{
		cout << "Tree Edge: " << u << " -> " << v << endl;
	}
	else
	{
		if (visited[v] == 1)
		{
			cout << "Back Edge: " << u << " -> " << v << endl;
		}
		else
		{
			cout << "Cross/Forward Edge: " << u << " -> " << v << endl;
		}
	}
}

void dfs_u(int v, Operation* op)
{
	visited[v] = 1;
	cout << v << " ";
	if (op) op->count();
	Node* actual = adjL[v];
	int u;
	while (actual != NULL)
	{
		if (actual == nullptr)
		{
			cout << "Invalid pointer encountered!" << endl;
			return;
		}
		//if (op) op->count();
		u = actual->vertex;
		typeOfEdge(v, u);
		if (op) op->count();
		if (visited[u] == 0 && u < n)
			parent[u]=v, dfs_u(u, op);
		//if (op) op->count();
		actual = actual->next;
	}
	if (op) op->count();
	visited[v] = 2;
}

void prettyprintDFS(int v, int depth = 0)
{
	visited[v] = 1;
	int i; Node* current=adjL[v];
	for (i = 0; i < depth; i++)
	{
		cout << "   ";
	}
	cout << v << endl;
	while (current)
	{
		if (visited[current->vertex] == 0)
		{
			//visited[current->vertex] = 1;
			prettyprintDFS(current->vertex, depth + 1);
		}
		current = current->next;
	}
}
void DFSPrettyprint(int start)
{
	std::fill(visited, visited + 300, 0); // Reset visited array
	cout << "DFS Tree Structure (Pretty Print):" << endl;
	prettyprintDFS(start); // Start recursive pretty print from root
}

void DFS(int start, Operation* op)
{
	std::fill(visited, visited + 200, 0);
	//memset(visited, 0, sizeof(visited));
	dfs_u(start, op);
	int i;
	for (i = 0; i < n; i = i + 1)
	{
		if (visited[i] == 0)
			dfs_u(i, op);
	}
}

void topoSort_u(int v, int st[], int& top)
{
	visited[v] = 1;
	Node* actual = adjL[v];
	while (actual)
	{
		if (visited[actual->vertex] == 0)
			topoSort_u(actual->vertex, st, top);
		actual = actual->next;
	}
	st[++top] = v;
}

void topoSort()
{
	int st[300], top = -1;
	std::fill(visited, visited + 300, 0);
	//memset(visited, 0, sizeof(visited));
	int i;
	for (i = 0; i < n; i++)
	{
		if (visited[i] == 0)
			topoSort_u(i, st, top);
	}
	cout << "Topological sort:" << endl;
	while (top >= 0)
	{
		cout << st[top--] << " ";
	}
	cout << endl;
}

void tarjan_u(int u, int index[], int lowLink[], int stack[], int& top, int inStack[], int& timeDiscovered, int strconncomp[])
{
	timeDiscovered++;
	index[u] = lowLink[u] = timeDiscovered;
	top++;
	stack[top] = u;
	inStack[u] = 1;
	Node* actual = adjL[u];
	int v;
	while (actual)
	{
		v = actual->vertex;
		if (index[v] == -1)
		{
			tarjan_u(v, index, lowLink, stack, top, inStack, timeDiscovered, strconncomp);
			lowLink[u] = min(lowLink[u], lowLink[v]);
		}
		else
		{
			if (inStack[v])
				lowLink[u] = min(lowLink[u], index[v]);
		}
		actual = actual->next;
	}
	if (lowLink[u] == index[u])
	{
		cout << "Strongly connected component: ";
		int v;
		do
		{
			v = stack[top];
			top--;
			inStack[v] = 0;
			strconncomp[v] = u;
			cout << v << ' ';
		} while (v != u);
		cout << endl;
	}
}
void tarkanStrConComp()
{
	int index[300], lowLink[300], stack[300], inStack[300], strconcomp[300], currentTime = 0, top = -1, i;
	std::fill(index, index + 300, -1);
	std::fill(lowLink, lowLink + 300, -1);
	std::fill(inStack, inStack + 300, 0);
	/*memset(index, -1, sizeof(index));
	memset(lowLink, -1, sizeof(lowLink));
	memset(inStack, 0, sizeof(inStack));*/
	for (i = 0; i < n; i++)
	{
		if (index[i] == -1)
			tarjan_u(i, index, lowLink, stack, top, inStack, currentTime, strconcomp);
	}
}
void generateR(int v, int e)
{
	initializeGraph();
	int added = 0, u, v2;
	while (added < e)
	{
		u = rand() % v;
		v2 = rand() % v;
		if (u != v2)
		{
			Node* actual = adjL[u];
			bool exists;
			exists = false;
			while (actual && exists == false)
			{
				if (actual->vertex == v2)
					exists = true;
				actual = actual->next;
			}
			if (actual && exists == 0)
			{
				added++;
				addEdge(u, v2);
			}
		}
	}
	cout << "Graph with " << e << " edges generated." << endl;

}

void DFSNormal(Operation* op, int v)
{
	int i;
	std::fill(visited, visited + 200, 0);
	//memset(visited, 0, sizeof(visited));
	//cout << "DFS Traversal Order:" << endl;
	for (i = 0; i < v; i++)
	{
		if (visited[i] == 0)
		{
			dfs_u(i, op);
		}
	}
	//cout << endl << endl;
}

void generateGraph2(int vert, int edg)
{
	srand(time(nullptr));
	initializeGraph();
	bool exists[200][200] = { false };
	int added = 0, u, v;
	int i;
	/*for (i = 0; i < vert; i++)
	{
		graph
	}*/
	while (added < edg&&added<vert*(vert-1))
	{
		/*u = rand() % vert;
		v = rand() % vert;
		if (u != v)
		{
			Node* current = adjL[u];
			bool exists = false;

			// Check if the edge already exists
			while (current != nullptr) {
				if (current->vertex == v) {
					exists = true;
					break;
				}
				current = current->next;
			}

			// Add the edge if it does not exist
			if (!exists) {
				addEdge(u, v);
				added++;
			}
			/*addEdge(u, v);
			exists[u][v] = true;
			added++;
		}
		else
		{
			if (u > 1)
				u--;
			else
				u++;
		}*/
		u = rand() % vert;
		v = rand() % vert;
		// Ensure u != v and edge doesn't already exist
		if (u != v && !exists[u][v])
		{
			addEdge(u, v);      // Add the edge
			exists[u][v] = true; // Mark the edge as existing
			added++;
		}
		
	}
	cout << "Graph with " << vert << " vertices and " << edg << " edges generated." << endl;
}

void performance()
{
	Profiler profiler("DFS");
	int e, v;
	for (e = 1000; e <= 4500; e = e + 100)
	{
		Operation op = profiler.createOperation("edges", e);
		n = 100;
		cout << "Start generating graph" << endl;
		//generateR(100, e);
		generateGraph2(100, e);
		cout << "Graph generation done" << endl;
		DFS(0, &op);
	}
	cout << "Edge performance analysis completed. Starting vertex performance analysis..." << endl;
	for (v = 100; v <= 200; v = v + 10)
	{
		Operation op2 = profiler.createOperation("vertices", v);
		n = v;
		int maxEdges = v * (v - 1); // Directed graph maximum edges
		int edgeCount = min(4500, maxEdges); // Adjust edge count dynamically
		//generateR(v, 4500);
		cout << "Start generating graph" << endl;
		generateGraph2(v, edgeCount);
		cout << "Graph generation done" << endl;
		DFS(0, &op2);
	}
	profiler.showReport();
}

void demo() {
	initializeGraph();
	n = 8;
	/*
		Graph structure:
			0 -> 1 -> 2 -> 0  (Back edge: 2 -> 0)
			0 -> 3
			3 -> 4 -> 5       (Tree edges)
			4 -> 6            (Forward edge)
			5 -> 6            (Cross edge)
			6 -> 7 -> 6       (Back edge: 7 -> 6)
	*/
	addEdge(0, 1);
	addEdge(1, 2);
	addEdge(2, 0);
	addEdge(0, 3);
	addEdge(3, 4);
	addEdge(4, 5);
	addEdge(4, 6);
	addEdge(5, 6);
	addEdge(6, 7);
	addEdge(7, 6);
	cout << "Initial Graph:" << endl;
	printG();
	cout << endl;
	DFS(0, NULL);
	DFSPrettyprint(0);
	//(NULL,n);
	cout << "Topological Sort Result:" << endl;
	topoSort();
	cout << endl;
	cout << "Strongly Connected Components (Tarjan's Algorithm):" << endl;
	tarkanStrConComp();
	cout << endl;
}

int main()
{
	demo();
	/*const int numVertices = 10;
	const int numEdges = 15;

	generateGraph2(numVertices, numEdges);
	n = numVertices;
	printG();*/
	//performance();
	return 0;
}