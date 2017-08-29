#include <bits/stdc++.h>
#include "trie.h"

Trie::Trie() {
    root = nullptr;
}

Node *Trie::newNode(int key, int num_child) {
    auto *node = (Node *) malloc(sizeof(node));
    node->key = key;
    node->num_child = num_child;
    node->next = (Node **) malloc(num_child * sizeof(Node *));
    for (int i = 0; i < num_child; i++) {
        node->next[i] = NULL;
    }
    return node;
}

Node *Trie::insertOne(Node *root, int key, int num_child) {
    int index = key - root->key - 1;
    if (root->next[index] == NULL) {
        Node *node = newNode(key, num_child);
        root->next[index] = node;
    }
    return root->next[index];
}

void Trie::insertAll(Node *root, vector<int> item_set, int k, int limit, int count) {
    for (int i = 0; i < k; i++) {
        root = insertOne(root, item_set[i], limit - item_set[i]);
    }
    root->count = count;
}

void Trie::insert(vector<int> item_set, int limit, int count) {
    insertAll(root, item_set, int(item_set.size()), limit, count);
}

int Trie::getCount(vector<int> item_set) {
    Node *tmp = root;
    int len = (int) item_set.size();
    int read = 0;
    for (int i = 0; i < len; i++) {
        int index = item_set[i] - read - 1;
        if (tmp->next[index] == NULL) {
            return 0;
        }
        tmp = tmp->next[index];
        read = item_set[i];
    }
    return tmp->count;
}
