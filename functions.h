#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <numeric> 
#include <map>
#include <set>
#include <algorithm> 
#include <ctime> 
#include <iterator>
#include <math.h> 
#include <random>

//a define variable that changes when the points are int or double
#define TP double
//a max number
#define INT_MAX 34456778
//a max number for ri of lsh euclidean hash function (g)
#define MAX 20
//a w number for h functions of lsh euclidean
#define W 2
//Alpha
#define A 15
//P (nearest neighbours)
#define P 20
//top virtual neighbours
#define S_V 2
//top normal neighbours
#define S_N 5
//number of clusters for virtual users
#define NUM_V 150

using namespace std;
using std::string;

#include "Dots.h"
#include "Tweets.h"

//http://www.cplusplus.com/forum/general/24427/
//Class for sorting a map
class sort_map{
  public:
  string key;
  double val;
};

//function for sorting a map
bool Sort_by(const sort_map& a ,const sort_map& b);
//function for sorting a map
bool Sort_by_up(const sort_map& a ,const sort_map& b);

/* Get arguments */
void get_args( int argc, char** argv, string& input_name, string& output_name, string& lexicon_name, string& coins_name, string& dataset_name, bool& validate);

/* Read Lexicon */
void read_lexicon_file(string lexicon_filename, map<string,double> *lexicon_map);

/* Read Coins */
void read_coins_file(string coins_filename, vector< vector<string> > * coins);

/* Read Input Data */
void read_input_file(string& input_name, map<int, Tweets* > * users, Tweets * tweets, int& p);

/* Read Dataset */
void read_dataset_file(string dataset_filename, vector<string> * in_ids, vector< vector<TP> > *points);

/* Create vector of coins for users */
void create_user_coins(map<int, Tweets* > * users, vector< vector<string> > * coins, vector<User_coins*> * user_coins, map<string, int>* uc_index, bool vrtl);

/* Create vector of coins for users in validation algorithm */
void create_user_coins_validation(map<int, Tweets* > * users, vector< vector<string> > * coins, vector<User_coins*> * user_coins, map<string, int>* uc_index, bool vrtl);

/* Cosine distance for points */
double cosine_distance(vector<TP> &x, vector<TP> &y);

/* Euclidean distance for points */
double euclidean_distance(vector<TP> &p, vector<TP> &v);

/* Internal product for points */
double internal_product(vector<TP> &p, vector<double> &v);

/* Clustering for virtual users */
void virtualclustering(vector< vector<TP> > *points, vector<string> * in_ids, Tweets * tweets, map<int, Tweets* > * v_users);

/* Print the results */
void print_results(vector<User_coins*> * user_coins, vector< vector<string> > * coins, double secs, int choice, ofstream& myfile_out);

/* Compute the minimum distance of the centroids */
double min_distance(vector<TP> &q, vector< vector<TP> > * points, int metric, int* index);

/* Functions of the 2nd project for clustering */

void initialization_random(vector< vector<TP> > *points, vector<string> * in_ids, vector < vector<TP> > *centroids, vector<string> * centroids_ids, int& num_clusters, int& n);

void Lloyds_assignment(Dots *dots, vector< vector<TP> > *points, vector < vector<TP> > *centroids, int& counter, int n, int metric);

void update_kmeans(Dots *dots, vector < vector<TP> > *centroids, int num_clusters, int dimension);

void update_kmedoid(Dots * dots, vector < vector<TP> > *centroids, vector<string> * centroids_ids, int num_clusters, int m);

void silhouette(vector < vector<TP> > *centroids, Dots * dots, double* sil, int metric, int num_clusters, int num);

/* Validation function for all the algorithms of the excercise */
void validation_function(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, map<string, int>* uc_index, string output_name, int dimension , int& p);

/* Deletion */
void deletion(map<string,double> *lexicon_map, vector< vector<string> > * coins, map<int, Tweets* > * users, map<int, Tweets* > * v_users, vector<string> * in_ids, vector< vector<TP> > *points, Tweets * tweets, vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, map<string, int>* uc_index);
#endif