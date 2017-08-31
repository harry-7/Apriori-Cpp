/**
 * Created by harry7 on 30/8/17.
 *
 * This file contains the code for main functionality of the apriori algorithm
 * The Apriori implemented here assumes two things
 * 1. All data given will fit into the memory
 * 2. Number of unique items is not so small
 * This code uses trie (more like a hash tree) data structure to store the counts for frequent item sets
 * configuration is read from config.csv which is present in the same folder as the executable is present
 *
 */


#include <bits/stdc++.h>
#include <tr1/unordered_map>

#include "trie.h"
#include "utilities.h"


using namespace std;
using namespace std::tr1;

/* ---------------- Global Variables ---------------- */
vector<int> frequencies; /* counts for all the items in the dataset */
unordered_map<string, int> map1; /* map from items to an integer */
vector<vector<int> > table; /* table which contains transactions of the data base in compressed format */
unordered_map<int, int> map2; /* map to be used for items which are frequent */
vector<int> single_item_sets; /* Vector to hold all the frequent 1 item sets */
vector<vector<int> > tri_array; /* Triangular array to hold the counts for 2 item sets */
Trie tree; /* Hash tree to hold the counts for all item sets */
vector<string> item_map; /* Reverse map from integer to the item */
vector<vector<vector<int> > > freq_item_sets; /* List of all frequent item sets of the data */
int min_count; /* minimum count of an item set for it to be frequent*/
Configuration config; /* struct to hold the configuration given in `config.csv` */

/* ---------------- Function Declarations  ---------------- */

/**
 * Removes the in frequent items from each record of the table
 */
void purgeTable();

/**
 * Makes a pass over all the records in the table and counts the frequency
 * of each pair in the record and updates those frequencies
 */
void updatePairFrequencies();

/**
 * Generates the next length (k+1) item sets from given length (k) item sets
 *
 * @param k current length of item sets
 * @return candidate item sets of length k+1 which can be frequent
 */
vector<vector<int> > aprioriGen(int k);

/**
 * Generates association rules from the frequent item sets present
 *
 * @param lhs left hand parts of association rules
 * @param rhs right hand parts of association rules
 * @return number of association rules generated
 */
int getAssociationRules(vector<vector<int> > &lhs, vector<vector<int> > &rhs);

/**
 * writes the frequent item sets to the output file as specified in config file
 * uses @link{getAssociationRules} to generate the association rules and writes
 * them to the output file if specified
 *
 * @param freq_item_count number of frequent item sets in the data
 */
void writeOutput(int freq_item_count);


/* ---------------- Driver program code ---------------- */

int main() {
    config = readConfig(); // Read the config from the config file

    /*
     * Read the input from the input file and prepare a map from items
     * and item map from integer to the item
     */
    int num_records = readInput(map1, item_map, config.input_file);
    table.resize((unsigned long) num_records);
    frequencies.resize(item_map.size());

    /*
     * Make the table from the input by the mapped values and count the frequencies
     * of all the items
     */
    makeTable(table, map1, frequencies, config.input_file);
    int num_items = int(item_map.size());
    tree.root = tree.newNode(0, num_items + 1);

    vector<vector<int> > empty_set;
    vector<int> temp;
    freq_item_sets.push_back(empty_set); // 0 length item-sets
    freq_item_sets.push_back(empty_set); // 1 length item-sets
    min_count = int(ceil(num_records * config.support));

    // Generate all the frequent 1-item sets by checking frequencies and update the tree
    int counter = 0;
    for (int i = 1; i < num_items; i++) {
        if (frequencies[i] >= min_count) {
            //cout << item_map[i] <<" " << frequencies[i] << " " << min_count << "\n";
            single_item_sets.push_back(i);
            temp.push_back(i);
            freq_item_sets[1].push_back(temp);
            tree.insert(temp, num_items + 1, frequencies[i]);
            temp.pop_back();
            map2[i] = counter + 1;
            counter++;
        }
    }

    /*
     * Iterate over all the record in the table and remove the infrequent items
     * from all the records and update the records in table
     */
    purgeTable();

    // Generating triangular array for use when generating two item sets
    tri_array.resize((unsigned long) (counter + 1));
    for (int i = 2; i <= counter; i++) {
        for (int j = 0; j < i; j++) {
            tri_array[i].push_back(0);
        }
    }

    // Go through the updated records and update the frequencies of each pair and update the tree
    updatePairFrequencies();

    // Generate the frequent 2 item sets by going through the pair frequencies
    freq_item_sets.push_back(empty_set); // for 2 item sets
    vector<int> pair_temp;
    for (int j = 1; j < counter; j++) {
        for (int i = j + 1; i <= counter; i++) {
            if (tri_array[i][j] >= min_count) {
                pair_temp.push_back(single_item_sets[j - 1]);
                pair_temp.push_back(single_item_sets[i - 1]);
                freq_item_sets[2].push_back(pair_temp);
                tree.insert(pair_temp, num_items + 1, tri_array[i][j]);
                pair_temp.pop_back();
                pair_temp.pop_back();
            }
        }
    }

    // Run apriori for remaining item set sizes using tree
    int freq_item_count = int(freq_item_sets[1].size() + freq_item_sets[2].size());
    // cerr << "2-item sets done with total count " << freq_item_count << "\n";
    int current = 3;
    vector<int> k_set_frequencies;
    while (freq_item_sets[current - 1].size() > 1) {
        vector<vector<int> > gen = aprioriGen(current - 1);
        // count frequencies for all the item sets returned by the aprioriGen
        k_set_frequencies.clear();
        for (int i = 0; i < gen.size(); i++) {
            k_set_frequencies.push_back(0);
        }
        // Iterate through all records and check if that record contains the item set
        int gen_size = int(gen.size());
        for (int i = 0; i < num_records; i++) {
            for (int j = 0; j < gen_size; j++) {
                int ind1 = 0;
                int ind2 = 0;
                int ind1_max = int(table[i].size());
                while (ind2 < current) {
                    while (ind1 < ind1_max && table[i][ind1] != gen[j][ind2]) {
                        ind1++;
                    }
                    if (ind1 == ind1_max) {
                        break;
                    }
                    ind2++;
                }
                if (ind2 == current) {
                    // All the items are present so increase the count
                    k_set_frequencies[j]++;
                }
            }
        }
        // Push the frequent item sets into the answer and insert into tree
        freq_item_sets.push_back(empty_set);
        int k_freq_item_count = 0;
        for (int j = 0; j < gen_size; j++) {
            if (k_set_frequencies[j] >= min_count) {
                freq_item_sets[current].push_back(gen[j]);
                tree.insert(gen[j], num_items + 1, k_set_frequencies[j]);
                k_freq_item_count++;
            }
        }
        freq_item_count += k_freq_item_count;
        current++;
    }
    writeOutput(freq_item_count);
    return 0;
}

/* ---------------- Implementation of above declared methods  ---------------- */

void purgeTable() {


    vector<int> temp;
    for (int i = 0; i < table.size(); i++) {
        //Go through each record of the table and keep track of items which are frequent
        for (vector<int>::iterator it = table[i].begin(); it != table[i].end(); it++) {
            if (frequencies[*it] >= min_count) {
                temp.push_back(*it);
            }
        }
        // Modify the record so that it contains only frequent items
        table[i].clear();
        for (vector<int>::iterator it = temp.begin(); it != temp.end(); it++) {
            table[i].push_back(*it);
        }
        temp.clear();
    }
}

void updatePairFrequencies() {
    /*
     * Go through each record in table and update the frequency for all
     * the pairs present in the record
     */
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int ind2 = map2[table[i][j]];
            for (int k = j + 1; k < table[i].size(); k++) {
                int ind1 = map2[table[i][k]];
                tri_array[ind1][ind2]++;
            }
        }
    }
}

vector<vector<int> > aprioriGen(int k) {
    int len = int(freq_item_sets[k].size());
    int i = 0, j = 1;
    vector<vector<int> > gen;
    /*
     * Iterate through the item set. For each index, get the max index until which
     * all items except the last one are same with the current item set. Then prepare
     * the union of these sets and check if any immediate subset is not frequent
     * If that is the case prune that itemset. Finally return all the candidate
     * frequent item sets of size k+1
     *
     */
    for (; i < len && j < len;) {
        for (; j < len; j++) {
            bool match = true;
            // check if the sets are same except the last item
            for (int l = 0; l < k - 1; l++) {
                if (freq_item_sets[k][i][l] != freq_item_sets[k][j][l]) {
                    // reached the non equal point before the last item
                    match = false;
                    break;
                }
            }
            if (!match) {
                break;
            }
        }
        vector<int> temp;
        for (int l = 0; l < k - 1; l++) {
            temp.push_back(freq_item_sets[k][i][l]);
        }
        /*
         * Iterate over possible pairs of item sets with all except the last item same
         * and generate the candidate item sets of size k + 1.
         */

        for (int ind1 = i; ind1 < j; ind1++) {
            for (int ind2 = ind1 + 1; ind2 < j; ind2++) {
                temp.push_back(freq_item_sets[k][ind1][k - 1]);
                temp.push_back(freq_item_sets[k][ind2][k - 1]);
                vector<int> tmp;
                bool frequent = true;
                /* Prune out the item set if any immediate subset is not frequent */
                for (int l = 0; l < k - 1; l++) {
                    for (int m = 0; m < k - 1; m++) {
                        if (l != m) {
                            tmp.push_back(temp[m]);
                        }
                    }
                    tmp.push_back(temp[k - 1]);
                    tmp.push_back(temp[k]);
                    if (tree.getCount(tmp) < min_count) {
                        frequent = false;
                        break;
                    }
                    tmp.clear();
                }
                if (frequent) {
                    // No immediate subset is not frequent so add this to list
                    gen.push_back(temp);
                }
                temp.pop_back();
                temp.pop_back();
            }
        }
        i = j;
        j = i + 1;
    }
    return gen;
}

void writeOutput(int freq_item_count) {
    ofstream output(config.output_file.c_str());
    output << freq_item_count << endl;
    for (int i = 0; i < freq_item_sets.size(); i++) {
        for (int j = 0; j < freq_item_sets[i].size(); j++) {
            int item_sz = int(freq_item_sets[i][j].size());
            for (int k = 0; k < item_sz - 1; k++) {
                output << item_map[freq_item_sets[i][j][k]] << ",";
            }
            output << item_map[freq_item_sets[i][j][item_sz - 1]] << "\n";
        }
    }
    if (config.flag == 0) {
        output.close();
        return;
    }
    // get the association rules from frequent item_sets
    vector<vector<int> > assoc_rules_lhs;
    vector<vector<int> > assoc_rules_rhs;
    int num_assoc_rules = getAssociationRules(assoc_rules_lhs, assoc_rules_rhs);
    output << num_assoc_rules << "\n";
    for (int i = 0; i < num_assoc_rules; i++) {
        int lhs_len = int(assoc_rules_lhs[i].size());
        for (int j = 0; j < lhs_len; j++) {
            output << item_map[assoc_rules_lhs[i][j]] << ",";
        }
        output << "=>,";
        int rhs_len = int(assoc_rules_rhs[i].size());
        for (int j = 0; j < rhs_len - 1; j++) {
            output << item_map[assoc_rules_rhs[i][j]] << ",";
        }
        output << item_map[assoc_rules_rhs[i][rhs_len - 1]] << "\n";
    }
    output.close();
}

int getAssociationRules(vector<vector<int> > &lhs, vector<vector<int> > &rhs) {
    /*
     *  Go through the each item set and pick a subset as lhs and remaining items
     *  and pick the remaining elements as rhs and calculate the confidence of
     *  the rule. If it has minimum confidence and then add those to lhs and rhs
     *  respectively.
     */
    int num_assoc_rules = 0;
    vector<int> lhs_items;
    vector<int> rhs_items;
    for (int item_sz = 2; item_sz < freq_item_sets.size(); item_sz++) {
        int num_sets = int(freq_item_sets[item_sz].size());
        int lim = (1 << item_sz) - 1; // number of subsets of a set of size n =  2^n
        for (int i = 0; i < num_sets; i++) {
            int sup = tree.getCount(freq_item_sets[item_sz][i]);
            for (int j = 1; j < lim; j++) {
                for (int k = 0; k < item_sz; k++) {
                    // check if the bit is set if yes add it to lhs or add it to rhs
                    if (j >> k & 1) {
                        lhs_items.push_back(freq_item_sets[item_sz][i][k]);
                    } else {
                        rhs_items.push_back(freq_item_sets[item_sz][i][k]);
                    }
                }
                // check for the support
                int lhs_sup = tree.getCount(lhs_items);
                int min_sup = int(ceil(config.confidence * lhs_sup));
                if (sup >= min_sup) {
                    num_assoc_rules++;
                    lhs.push_back(lhs_items);
                    rhs.push_back(rhs_items);
                }
                lhs_items.clear();
                rhs_items.clear();
            }
        }
    }
    return num_assoc_rules;
}


void printTable() {
    // For debugging purposes
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << item_map[table[i][j]] << ", ";
        }
        cout << "\n";
    }
}