/*
 * Created by harry7 on 30/8/17.
 */

#ifndef CPP_UTILITIES_H
#define CPP_UTILITIES_H

#include <bits/stdc++.h>
#include <tr1/unordered_map>


using namespace std;
using namespace std::tr1;

typedef struct Configuration {
    string input_file;
    string output_file;
    double support;
    double confidence;
    int flag;
} Configuration;

Configuration readConfig();

int readInput(unordered_map<string, int> &map, vector<string> &item_map, string input_file);
void makeTable(vector<vector<int> > &table, unordered_map<string, int> &map1, vector<int> &frequencies, string input_file);
#endif //CPP_UTILITIES_H
