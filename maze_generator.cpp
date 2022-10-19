#include <iostream>
#include <bits/stdc++.h>
#define ROWS 6
#define COLS 8

using namespace std;

/**
 * WALLS
 */

struct wall {
    int i,j; // cell 1 coordinates
    int h,k; // cell 2 coordinates
};

vector<wall> wallslist;

/**
 * DISJOINT SETS
 */

class DisjSets {

public:

    int n;
    int *parent;

    // Creates n single item sets
    void init() const{
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Finds set of item x
    int Find(int x){
        // Finds the representative of the set that x is an element of
        if (parent[x] != x) {
            // if x is not the parent of itself then x is not the representative of his set
            parent[x] = Find(parent[x]);
        }
        // so we recursively call Find on its parent and move i's node directly under the representative of this set
        return parent[x];
    }

    // Do union of two sets represented by x and y.
    void Union(int i, int j) {
        // Find the representatives (or the root nodes) for the set that includes i
        int irep = this->Find(i);
        // And do the same for the set that includes j
        int jrep = this->Find(j);
        // Make the parent of i’s representative be j’s  representative effectively moving all of i’s set into j’s set)
        this->parent[irep] = jrep;
    }

    // Return true if two elements are in the same set, false otherwise
    bool SameSet(int x, int y){
        return Find(x) == Find(y);
    }
};

DisjSets sets;

/**
 * DFS
 */

vector<vector<int> > adj; // adjacency matrix

void addEdge(int x, int y){ // function to add edge to the graph
    adj[x][y] = 1;
    adj[y][x] = 1;
}

void dfs(int source, vector<bool>& visited){ // function to perform DFS on the graph

    // Print the current node
    cout << source << " ";

    // Set current node as visited
    visited[source] = true;

    // For every node of the graph
    for (int i = 0; i < adj[source].size(); i++) {
        // If some node is adjacent to the current node and it has not already been visited
        if (adj[source][i] == 1 && (!visited[i])) {
            dfs(i, visited);
        }
    }
}

void performDFS(int numcells){
    // initialize adjacency matrix with as many vertices as the number of cells
    adj = vector<vector<int> >(numcells, vector<int>(numcells, 0));

    // create the graph on which perform DFS
    for(int i=0; i<numcells; i++){
        addEdge(sets.parent[i], i);
    }

    // Visited vector to so that a vertex is not visited more than once
    // Initializing the vector to false as no vertex is visited at the beginning
    vector<bool> visited(numcells, false);

    // Perform DFS
    printf("performing DFS: start node 0, goal node %d\nnodes visited: \n", numcells-1);
    dfs(0, visited);
}

/**
 * RANDOM MAZE
 */

void ranMaze(int rows, int columns){
    int numcells = rows * columns;
    int start = 0; // first cell of the maze
    int goal = numcells-1; // last cell of the maze

    // disjoint sets initialization
    sets.n=numcells;
    sets.parent = new int[numcells];
    sets.init();

    srand ( time(nullptr) );
    while (!sets.SameSet(start, goal)){
        // select a wall at random
        int randindex = rand() % wallslist.size(); //generates a random number between 0 and walls list size
        wall randwall = wallslist.at(randindex);

        printf("random wall: cell(%d,%d) | cell(%d, %d) \n", randwall.i, randwall.j, randwall.h, randwall.k);

        // retrieve the two adjacent cells of the wall
        int cell1 = (randwall.i*columns) + randwall.j;
        int cell2 = (randwall.h*columns) + randwall.k;

        printf("cells adjacent to the wall: %d | %d\n", cell1, cell2);

        if (!sets.SameSet(cell1, cell2)){ // check if cell2 is reachable from cell1

            printf("cells are not in the same set..\n");

            // remove the wall from the maze
            wallslist.erase(wallslist.begin()+randindex);

            // put the two cells in the same set
            sets.Union(cell1, cell2);
            printf("destroying wall %d | %d ...\n", cell1, cell2);

            printf("parent array after the union:  ");
            for (int i=0; i<numcells; i++){
                if (i != numcells - 1) printf("%d , ", sets.parent[i]);
                else printf("%d \n\n\n", sets.parent[i]);
            }
        } else printf("cells are already in the same set\n\n\n");
    }
}

void retrieveWalls(int rows, int columns){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            wall tmp{};
            if (j + 1 < columns) {
                tmp.i = i;
                tmp.j = j;
                tmp.h = i;
                tmp.k = j + 1;
                wallslist.push_back(tmp);
            }

            if (i + 1 < rows) {
                tmp.i = i;
                tmp.j = j;
                tmp.h = i + 1;
                tmp.k = j;
                wallslist.push_back(tmp);
            }

        }
    }
}

void printMaze(){

    bool next = false;
    bool under = false;

    printf("\nGenerated Maze:\n");

    string roof;
    roof += " ";
    string one_roof = "__";
    for (int i = 0; i < COLS; i++){
        roof += one_roof;
    }

    cout << roof << endl;

    for (int i = 0; i < ROWS; i++){
        string row;
        row += "|";
        for (int j = 0; j < COLS; j++){

            for (wall w: wallslist){
                if (i == w.i && j==w.j && i+1==w.h && j==w.k && i+1<ROWS){
                    under = true;
                }
                if (i == w.i && j==w.j && i==w.h && j+1==w.k && j+1<COLS){
                    next = true;
                }
            }

            if (next && under){
                row += "_|";
            } else if (next){
                row += " |";
            } else if (under){
                row += "_ ";
            } else row += "  ";

            next = false;
            under = false;

        }
        row += "|";
        cout << row << endl;
    }
    
    roof = " ";
    one_roof = "--";
    for (int i = 0; i < COLS; i++){
        roof += one_roof;
    }
    cout << roof << endl;

}

int main() {

    retrieveWalls(ROWS, COLS);

    ranMaze(ROWS, COLS);

    int numcells = ROWS*COLS;

    printf("final parent array:\n");
    for (int i=0; i<numcells; i++){
        if (i != numcells - 1) printf("%d , ", sets.parent[i]);
        else printf("%d \n\n", sets.parent[i]);
    }

    performDFS(ROWS*COLS);

    printMaze();

    return 0;
}
