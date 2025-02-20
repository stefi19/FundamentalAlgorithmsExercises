#include <stdlib.h>
#include <string.h>
#include "Profiler.h"
#include "bfs.h"

#define PERETE 1

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int nrOfNeighbours = 0, i;
    const int dx[] = { -1, 1, 0, 0 }, dy[] = { 0, 0, -1, 1 };
    for (i = 0; i < 4; i++)
    {
        int newX = p.row + dx[i], newY = p.col + dy[i];
        if (newX >= 0 && newX < grid->rows && newY >= 0 && newY < grid->cols)
        {
            //the cell is not a wall
            if (grid->mat[newX][newY] != PERETE)
            {
                neighb[nrOfNeighbours].row = newX;
                neighb[nrOfNeighbours].col = newY;
                nrOfNeighbours++;
            }
        }
    }
    return nrOfNeighbours;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    int i;
    for (i = 0; i < graph->nrNodes; i++)
    {
        Node* n = graph->v[i];
        n->color = COLOR_WHITE;
        n->dist = -1;
        n->parent = NULL;
        if (op)
            op->count();
    }
    s->parent = NULL;
    s->dist = 0;
    s->color = COLOR_GRAY;
    if (op)
        op->count(3);
    Node** q = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    int first = 0, last = 0;
    q[last] = s;//start node
    last++;
    if (op) op->count();
    while (first < last)
    {
        Node* curr = q[first];
        first++;
        if (op) op->count();
        for (i = 0; i < curr->adjSize; i++)
        {
            Node* neigh = curr->adj[i];
            if (neigh->color == COLOR_WHITE)
            {
                neigh->parent = curr;
                neigh->dist = curr->dist + 1;
                neigh->color = COLOR_GRAY;
                if (op) op->count(3);
                q[last] = neigh;
                last++;
                if (op) op->count();
            }
            if (op) op->count();
        }
        curr->color = COLOR_BLACK;
        if (op) op->count();
    }
    free(q);
}

void ppTree(int* p, Point* repr, int n, int root, int l)
{
    int i;
    for (i = 0; i < l; i++)
        printf("   ");
    printf("(%d, %d)\n", repr[root].row, repr[root].col);
    for (i = 0; i < n; i++)
    {
        if (p[i] == root)
            ppTree(p, repr, n, i, l + 1);
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int i;
        for (i = 0; i < n; i++)
        {
            if (p[i] == -1)
            {
                ppTree(p, repr, n, i, 0); break;
            }
        }
    }
    else
        printf("Error constructing BFS tree.\n");

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    if (start == end)
    {
        path[0] = end;
        return 0;
    }
    bfs(graph, start, NULL);
    if (end->color != COLOR_BLACK)
        return -1;
    int pathL = 0, i;
    Node* curr = end; //start from end and go to parent
    while (curr)
    {
        path[pathL] = curr;
        pathL++;
        curr = curr->parent;
    }
    for (i = 0; i < pathL / 2; i++)
    {
        Node* t = path[i];
        path[i] = path[pathL - i - 1];
        path[pathL - i - 1] = t;
    }
    return pathL;
}

void generateRandomCGraph(Graph* g, int edgesN)
{
    int i,edgAdd=g->nrNodes-1,u,v,exist;
    //create a spanning tree
    for (i = 0; i < g->nrNodes - 1; i++)
    {
        if (g->v[i]->adj == NULL)
            g->v[i]->adj = (Node**)malloc(sizeof(Node*));
        g->v[i]->adj[0] = g->v[i + 1];
        g->v[i]->adjSize = 1;
    }
    g->v[g->nrNodes - 1]->adjSize = 0;
    while (edgAdd < edgesN)
    {
        u = rand() % g->nrNodes;
        v = rand() % g->nrNodes;
        if (u != v)
        {
            exist = 0;
            for (i = 0; i < g->v[u]->adjSize; i++)
            {
                if (g->v[u]->adj[i] == g->v[v])
                {
                    exist = 1;
                    break;
                }
            }
            if (exist == 0)
            {
                //add edge
                g->v[u]->adj = (Node**)realloc(g->v[u]->adj, (g->v[u]->adjSize + 1) * sizeof(Node*));
                g->v[u]->adj[g->v[u]->adjSize++] = g->v[v];
                edgAdd++;
            }
        }
    }
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generateRandomCGraph(&graph, n);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generateRandomCGraph(&graph, 4500);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}

#define CHESSBOARDSIZE 8

const int knightMove[8][2] = { {2,1},{2,-1},{-2,1},{-2, -1},{1,2},{1,-2},{-1,2},{-1,-2} };

int validPoz(int r, int c)
{
    if (r >= 00 && r <= 8 && c >= 0 && c < 8)
        return 1;
    return 0;
}
void bfsKnight(int startR, int startC, int visited[8][8])
{
    int q[64][2], first = 0, last = 0;
    q[last][0] = startR;
    q[last][1] = startC;
    last++;
    visited[startR][startC] = 1;
    while (first < last)
    {
        int currR = q[first][0], currC = q[first][1], i;
        first++;
        for (i = 0; i < 8; i++)
        {
            int newR = currR + knightMove[i][0], newC = currC + knightMove[i][1];
            if (validPoz(newR, newC) && visited[newR][newC] == 0)
            {
                visited[newR][newC] = 1;
                q[last][0] = newR;
                q[last][1] = newC;
                last++;
            }
        }
    }
}
int checkItCanReachAllPoints()
{
    int visited[8][8], r, c;
    memset(visited, 0, sizeof(visited));
    bfsKnight(0, 0, visited);
    for (r = 0; r < 8; r++)
    {
        for (c = 0; c < 8; c++)
        {
            if (visited[r][c] == 0)
                return 0;
        }
    }
    return 1;
}
