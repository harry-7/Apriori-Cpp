/*
 * Created by harry7 on 30/8/17.
 *
 * This file contains the code which implements the functions described in utilities.h
 */

#include <bits/stdc++.h>
#include <tr1/unordered_map>
#include "utilities.h"

#define DELIM ','

using namespace std;
using namespace std::tr1;

int readInput(unordered_map<string, int> &map, vector<string> &item_map, string input_file) {
    ifstream input(input_file.c_str());
    int num_records = 0;
    set<string> items;
    for (string line; getline(input, line);) {
        unsigned long len = line.length();
        for (int i = 0; i < len; i++) {
            // Split words based on ','
            string word = "";
            for (; i < len && line[i] != DELIM; ++i) {
                word += line[i];
            }
            items.insert(word);
        }
        num_records++;
    }
    int item_count = 1;
    item_map.push_back("");
    set<string>::iterator it;
    for (it = items.begin(); it != items.end(); it++) {
        map[*it] = item_count;
        item_map.push_back(*it);
        item_count++;
    }
    return num_records;
}

Configuration readConfig() {
    Configuration config;
    config.flag = 1;
    ifstream file("config.csv");
    for (string line; getline(file, line);) {
        unsigned long long len = line.length();
        switch (line[0]) {
            case 'i' :
                config.input_file = line.substr(6, len - 6);
                break;
            case 'o':
                config.output_file = line.substr(7, len - 7);
                break;
            case 's':
                config.support = atof(line.substr(8, len - 8).c_str());
                break;
            case 'c':
                config.confidence = atof(line.substr(11, len - 11).c_str());
                break;
            case 'f':
                config.flag = line[5] - '0';
                break;
            default:
                cerr << "Error in reading config file\n";
                exit(-1);
        }
    }
    return config;
}

void
makeTable(vector<vector<int> > &table, unordered_map<string, int> &map1, vector<int> &frequencies, string input_file) {
    ifstream input(input_file.c_str());
    int index = 0;
    set<int> set1;
    for (string line; getline(input, line); ++index) {
        unsigned long len = line.length();
        for (int i = 0; i < len; i++) {
            string word = "";
            for (; i < len && line[i] != DELIM; ++i) {
                word += line[i];
            }
            set1.insert(map1[word]);
        }
        for (set<int>::iterator it = set1.begin(); it != set1.end(); it++) {
            table[index].push_back(*it);
            frequencies[*it]++;
        }
        set1.clear();
    }
}