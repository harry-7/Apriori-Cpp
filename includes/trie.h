/**
 * Created by harry7 on 30/8/17.
 *
 * This file contains the definitions for the Tree implementation to be used
 * by the apriori algorithm
 */

#ifndef CPP_TRIE_H
#define CPP_TRIE_H

#include <bits/stdc++.h>

using namespace std;
typedef struct Node {
    int key; /* Key of the item */
    int num_child; /* Number of children in this node */
    int count; /* number of item this node may occur in data */
    struct Node **next; /* pointers to the children */
} Node;

class Trie {
public:
    Node *root; /* root of the tree */

    Trie();

    Node *newNode(int key, int num_child);

    Node *insertOne(Node *root, int key, int num_child);

    void insertAll(Node *root, vector<int> item_set, int k, int limit, int count);

    void insert(vector<int> item_set, int limit, int count);

    int getCount(vector<int> item_set);
};


#endif //CPP_TRIE_H
