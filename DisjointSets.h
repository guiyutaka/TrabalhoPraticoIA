#pragma once

#include <unordered_set>
#include <unordered_map>
#include <list>

using namespace std;

// Disjoint sets
typedef int Item;

class DisjointNode;

class DisjointSet {
public:
    DisjointNode *root;
    int count;

    DisjointSet(DisjointNode *root);
    DisjointNode *begin();
};

class DisjointNode {
public:
    friend class DisjointSets;

    Item item;
    DisjointNode *parent;
    std::unordered_set<DisjointNode *> children;

    DisjointNode(Item item);
    DisjointNode *find();
    DisjointNode *next();
private:
    int rank;
    std::list<DisjointSet>::iterator iterator;

    DisjointNode *setParent(DisjointNode *parent);
};

class DisjointSets {
public:
    std::list<DisjointSet> sets;

    ~DisjointSets();
    void add(Item item);
    DisjointNode *find(Item item);
    bool join(Item item1, Item item2);
private:
    std::unordered_map<Item, DisjointNode *> nodes;

    void destroy(DisjointNode *node);
};