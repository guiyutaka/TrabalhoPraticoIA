#include "DisjointSets.h"

#include <unordered_set>

using namespace std;

DisjointSet::DisjointSet(DisjointNode * root) {
    this->root = root;
    this->count = 1;
}

DisjointNode *DisjointSet::begin() {
    DisjointNode *node = root;

    while (!node->children.empty())
        node = *node->children.begin();

    return node;
}

DisjointNode *DisjointNode::next() {
    DisjointNode *node = this;

    // Find first sibling
    do {
        DisjointNode *parent = node->parent;
        if (parent == NULL)
            return NULL;

        auto sibling = std::next(parent->children.find(node));
        if (sibling != parent->children.end()) {
            node = *sibling;
            break;
        }

        node = parent;
    } while (node != NULL);

    // Go to left-most child
    while (!node->children.empty())
        node = *node->children.begin();

    return node;
}

DisjointNode::DisjointNode(Item item) {
    this->rank = 0;
    this->item = item;
    this->parent = NULL;
}

DisjointNode * DisjointNode::find() {
    if (parent == NULL)
        return this;

    return setParent(parent->find());
}

DisjointNode * DisjointNode::setParent(DisjointNode * parent) {
    if (this->parent == parent)
        return parent;

    if (this->parent != NULL)
        this->parent->children.erase(this);

    parent->children.insert(this);
    return this->parent = parent;
}

DisjointSets::~DisjointSets() {
    unordered_set<DisjointNode *> roots;
    for (auto node : nodes)
        roots.insert(node.second->find());

    for (auto info : sets)
        destroy(info.root);
}

void DisjointSets::add(Item item) {
    sets.push_back(DisjointSet(nodes[item] = new DisjointNode(item)));
    sets.back().root->iterator = prev(sets.end());
}

DisjointNode * DisjointSets::find(Item item) {
    return nodes[item]->find();
}

bool DisjointSets::join(Item item1, Item item2) {
    DisjointNode *parent = find(item1);
    DisjointNode *child = find(item2);

    if (child == parent)
        return false;

    if (child->rank < parent->rank)
        swap(child, parent);
    if (child->rank == parent->rank)
        ++parent->rank;

    child->parent = parent;
    parent->children.insert(child);
    parent->iterator->count += child->iterator->count;
    sets.erase(child->iterator);

    return true;
}

void DisjointSets::destroy(DisjointNode * node) {
    for (auto child : node->children)
        destroy(child);

    delete node;
}