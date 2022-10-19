#include <benchmark/benchmark.h>
#include <iostream>
#include <bits/stdc++.h>

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
 * RANDOM MAZE
 */

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

void ranMaze(int rows, int columns){
    int numcells = rows * columns;
    int start = 0; // first cell of the maze
    int goal = numcells-1; // last cell of the maze
    
    // disjoint sets initialization
    sets.n=numcells;
    sets.parent = new int[numcells];
    sets.init();

    retrieveWalls(rows, columns);

    srand ( time(nullptr) );
    while (!sets.SameSet(start, goal)){
        // select a wall at random
        int randindex = rand() % wallslist.size(); //generates a random number between 0 and walls list size
        wall randwall = wallslist.at(randindex);

        // retrieve the two adjacent cells of the wall
        int cell1 = (randwall.i*columns) + randwall.j;
        int cell2 = (randwall.h*columns) + randwall.k;

        if (!sets.SameSet(cell1, cell2)){ // check if cell2 is reachable from cell1

            // remove the wall from the maze
            wallslist.erase(wallslist.begin()+randindex);

            // put the two cells in the same set
            sets.Union(cell1, cell2);
        }
    }
}

static void BM_ranMaze(benchmark::State& state) {
  int rows = state.range(0);
  int columns = state.range(0);

  for (auto _ : state) {
    ranMaze(rows, columns);
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_ranMaze)
    ->RangeMultiplier(2)
    ->Range(2,256)
    ->Complexity();

BENCHMARK_MAIN();
