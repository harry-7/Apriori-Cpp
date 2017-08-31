/*
 * Created by harry7 on 30/8/17.
 *
 * This file contains definition of functions which assist the apriori algorithm
 */

#ifndef CPP_UTILITIES_H
#define CPP_UTILITIES_H

#include <bits/stdc++.h>
#include <tr1/unordered_map>


using namespace std;
using namespace std::tr1;

typedef struct Configuration {
    string input_file; /* path of the input file */
    string output_file; /* path of the file for the output to be written*/
    double support; /* Minimum support needed for an item set to be frequent */
    double confidence; /* Minimum confidence needed for an association rule */
    int flag; /* Flag specifying the format of output. If the flag is one
               * we have to output both frequent item sets and association rules
               * else output only the frequent item sets
               */
} Configuration;

Configuration readConfig();

/**
 * reads the file from the path given as input_file and prepares a map from items
 * to an integer and inverse map from integer to the item
 *
 * @param map map from string to integer
 * @param item_map inverse map from integer to string
 * @param input_file string containing the path for input file
 * @return number of records in the database
 */
int readInput(unordered_map<string, int> &map, vector<string> &item_map, string input_file);

/**
  Makes the table of transaction using the mapped indices and counts the frequencies
  of single item sets in the table

  @param table the data structure holding all the records in database
  @param map1 map from item to the integer
  @param frequencies data structure which should hold the frequency count of single items
  @param input_file string containing the path for input file
 */
void
makeTable(vector<vector<int> > &table, unordered_map<string, int> &map1, vector<int> &frequencies, string input_file);

#endif //CPP_UTILITIES_H
