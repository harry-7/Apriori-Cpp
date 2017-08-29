/*
 * Created by harry7 on 30/8/17.
 */

#ifndef CPP_TRIE_H
#define CPP_TRIE_H

#include <bits/stdc++.h>

using namespace std;
typedef struct Node {
    int key;
    int num_child;
    int count;
    struct Node **next;
} Node;

class Trie {
public:
    Node *root;

    Trie();

    Node *newNode(int key, int num_child);

    Node *insertOne(Node *root, int key, int num_child);

    void insertAll(Node *root, vector<int> item_set, int k, int limit, int count);

    void insert(vector<int> item_set, int limit, int count);

    int getCount(vector<int> item_set);
};


#endif //CPP_TRIE_H
