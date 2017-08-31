#include <bits/stdc++.h>
#include "trie.h"

/**
 * Default constructor
 */
Trie::Trie() {
    root = NULL;
}

/**
 * constructs a new node and returns its pointer
 *
 * @param key key of the new node
 * @param num_child number of children to be present in new node
 * @return pointer to the node that is created
 */
Node *Trie::newNode(int key, int num_child) {
    Node *node = (Node *) malloc(sizeof(node));
    node->key = key;
    node->num_child = num_child;
    node->next = (Node **) malloc(num_child * sizeof(Node *));
    for (int i = 0; i < num_child; i++) {
        node->next[i] = NULL;
    }
    return node;
}

/**
 * inserts a new node as a child at the specified index at a node if it is not present already
 * and returns the pointer to that node
 *
 * @param root parent node
 * @param key key at which child has to be inserted
 * @param num_child number of children to be present at the child node
 * @return pointer to the node at the specified index of the parent node
 */
Node *Trie::insertOne(Node *root, int key, int num_child) {
    int index = key - root->key - 1;
    if (root->next[index] == NULL) {
        Node *node = newNode(key, num_child);
        root->next[index] = node;
    }
    return root->next[index];
}

/**
 * given set of items and a parent node insert all the elements at appropriate
 * indices of tree
 *
 * @param root node at which item hierarchy starts
 * @param item_set set of items to be inserted into the tree
 * @param k length of item set
 * @param limit limit on the number of children it can have
 * @param count
 */
void Trie::insertAll(Node *root, vector<int> item_set, int k, int limit, int count) {
    for (int i = 0; i < k; i++) {
        root = insertOne(root, item_set[i], limit - item_set[i]);
    }
    root->count = count;
}

/**
 * Insert the item set into the given tree
 *
 * @param item_set item set to be inserted
 * @param limit limit specifying number of children that node can have
 * @param count frequency of the item set
 */
void Trie::insert(vector<int> item_set, int limit, int count) {
    insertAll(root, item_set, int(item_set.size()), limit, count);
}

/**
 * Given an item set return its count
 *
 * @param item_set item set for which count is to be obtained
 * @return count of the item set
 */
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
