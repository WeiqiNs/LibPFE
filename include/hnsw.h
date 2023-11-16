#pragma once

#include <set>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "sym_ipre.hpp"

using namespace std;


struct Item {
    // The ciphertext as value.
    Ct value;

    // Compute distance between item with something else.
    int dist(Item &other, Key key, int size, int bound) const;
};


struct HNSWGraph {
    // Constructor.
    HNSWGraph(int NN, int MN, int MNZ, int SN, int ML, Key key, int size, int bound);

    /* HNSW related settings. */
    // Number of neighbors.
    int NN;
    // Max number of neighbors in layers >= 1.
    int MN;
    // Max number of neighbors in layers 0.
    int MNZ;
    // search numbers in construction (efConstruction).
    int SN;
    // Max number of layers.
    int ML;
    // number of items
    int numItem;
    // enter node id
    int enterNode{};
    // actual vector of the items
    vector<Item> items;
    // adjacent edge lists in each layer
    vector<unordered_map<int, vector<int>>> layerEdgeLists;
    // The default generator.
    default_random_engine generator;

    /* For the IPRE scheme. */
    Key key;
    int size;
    int bound;

    /* Methods. */
    void insert(Item &q);

    void addEdge(int st, int ed, int lc);

    vector<int> search(Item &q, int K);

    vector<int> searchLayer(Item &q, int ep, int ef, int lc);
};
