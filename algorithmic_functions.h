#ifndef __ALGORITHMIC_FUNCTIONS__
#define __ALGORITHMIC_FUNCTIONS__
#include "hash.h"

/*----------LSH-Cosine Algorithm----------*/
/* Search with the use of LSH */
void Search_lsh_tables(vector<User_coins *> * uc, int index, map <string, double> *map_index, HashTable ** table, GFunctions ** g, int L);

/* Insertion in a LSH table */
void insert_lsh_table(HashTable ** table, GFunctions ** fs, vector<User_coins*> * user_coins, map<string, vector<double> *> * map_vector, int dimension, int L, int k);

/*Find P best neighbours */
void P_best_neighbours(map <string, double> *map_index, int& p);

/* Lsh-cosine algorithm */
void lsh_cosine_algorithm(vector<User_coins*> * user_coins, int dimension, int& p);

/* Lsh-cosine algorithm for vaidation */
double lsh_cosine_algorithm_validation(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int& p, int f);


/*----------Clustering (Euclidean Metric) Algorithm----------*/
/* Clustering algorithm */
void clustering_algorithm(vector<User_coins*> * user_coins, int dimension);

/* Clustering algorithm for validation */
double clustering_algorithm_validation(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int f);

/* Clustering */
void simple_clustering(Dots * dots, vector< vector<TP> > *points, vector<string> * in_ids, int dimension, int n);

/* Creation of the structs(points, ids) for clustering by users */
void create_struct_for_clustering(vector<User_coins*> * user_coins, vector<string> * in_ids, vector<vector<TP> > *points, map<string, vector<double> *> * map_vector);

/* Find neighbours that belong at the same cluster with the user */
void find_neighbours_from_clustering(vector<User_coins *> * user_coin, int ind, Dots* dots, map <string, double> *map_index);


/*----------LSH-Cosine using Virtual Users Algorithm----------*/
/* Lsh-cosine algorithm using virtual users */
void lsh_cosine_algorithm_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, int dimension , int &p);

/* Lsh-cosine algorithm using virtual users for validation*/
double lsh_cosine_algorithm_virtual_validation(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int& p, int f);


/*----------Clustering using Virtual Users Algorithm----------*/
/* Clustering algorithm using virtual users */
void clustering_algorithm_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, int dimension);

/* Clustering algorithm using virtual users for validation*/
double clustering_algorithm_virtual_validation(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int f);

/* Find neighbours (virtual users) that belong at the same cluster with the user */
void find_neighbours_from_clustering_virtual( User_coins * user, Dots* dots, map <string, double> *map_index);

/* Creation of the structs(points, ids) for clustering by users and virtual users */
void create_struct_for_clustering_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<string> * in_ids, vector<vector<TP> > *points, map<string, vector<double> *> * map_vector);


/*----------Functions----------*/
/*Creation of a set of pairs for validation algorithm */
void create_set_of_pairs(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs);

/* Normalazation and no normalazation of the user's coins vector */
void n_normalazation(vector<User_coins*> * user_coins);

/* Compute the final coins for recommendation */
void final_coins(User_coins * user_coins, map<string, vector<double> *> * map_vector, map <string, double> *map_index, int choice, bool vrtl);

/*Compute the final coins for validation */
void final_coins_validation(User_coins * user_coins, map<string, vector<double> *> * map_vector, map <string, double> *map_index, map<string , vector<double> > *map_final, int choice);

/* Create virtual users */
void create_virtual_users(Dots* dots, map<int, Tweets* > * users, Tweets * tweets, int num_clusters);

/* Computation of Mean Absolute Error */
double compute_mae(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, map<string , vector<double> > *map_final, int f);

/* Reset users for validation algorithm */
void reset_users(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs,  map<string, int>* uc_index, int f);

/* Set users for validation algorithm */
void set_users(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs,  map<string, int>* uc_index,  int f);

#endif