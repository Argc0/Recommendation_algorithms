#include "functions.h"
#include "algorithmic_functions.h"

//lsh-cosine algorithm
void lsh_cosine_algorithm(vector<User_coins*> * user_coins, int dimension, int& p){
  //default numbers
  int L=5, k=4;
  HashTable **table = new HashTable*[L]();
  GFunctions ** fs = new GFunctions*[L]();
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();

  insert_lsh_table(table, fs, user_coins, map_vector, dimension, L, k);

  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    Search_lsh_tables(user_coins, i, map_index, table, fs, L);
    //find final results
    P_best_neighbours(map_index, p);
    //compute neighbor coins
    final_coins(user_coins[0][i], map_vector, map_index, 0, false);
    map_index->clear();
  }

  //deletion
  for(int i=0; i<L; i++)
    delete fs[i];
  delete[] fs;

  for(int i=0; i<L; i++)
    delete table[i];
  delete[] table;

  map_vector->clear();
  delete map_vector;

  map_index->clear();
  delete map_index;
  
}

//lsh-cosine algorithm for virtual users
void lsh_cosine_algorithm_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, int dimension, int& p){
  //default numbers
  int L=5, k=4;
  HashTable **table = new HashTable*[L]();
  GFunctions ** fs = new GFunctions*[L]();
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  
  insert_lsh_table(table, fs, user_coins_v, map_vector, dimension, L, k);

  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    Search_lsh_tables(user_coins, i, map_index, table, fs, L);
    //find final results
    P_best_neighbours(map_index, p);
    //compute neighbor coins
    final_coins(user_coins[0][i], map_vector, map_index, 0, true);
    map_index->clear();
  }

  //deletion
  for(int i=0; i<L; i++)
    delete fs[i];
  delete[] fs;

  for(int i=0; i<L; i++)
    delete table[i];
  delete[] table;

  map_vector->clear();
  delete map_vector;

  map_index->clear();
  delete map_index;
}

//create users with vector of coins
void create_user_coins(map<int, Tweets* > * users, vector< vector<string> > * coins, vector<User_coins*> * user_coins, map<string, int>* uc_index, bool vrtl){
  Tweets * t;
  double value;
  string id;
  int flag = 0;
  for(map<int, Tweets *>::iterator it = users->begin(); it != users->end(); ++it){
    t = it->second;
    User_coins * uc;
    if(vrtl){
      //adda the string v to stand out the normal users
      string v = "v";
      uc = new User_coins(v+to_string(it->first));
    }else
      uc = new User_coins(to_string(it->first));
    flag = 0;
    for(vector<vector<string>>::size_type c = 0; c < coins->size(); c++ ){
      value = t->find_coins(coins[0][c]);
      uc->add_value(value);
      if(isnan(value))
        uc->add_index_un(c);
      else
        uc->add_index(c);
      if(!isnan(value) && (value != 0.0))
        flag = 1;
    }
    if(((long unsigned int)uc->returnSize_of_Unknown_Coins() != coins->size()) && (flag == 1))
      user_coins->push_back(uc);
    else
      delete uc;
  }
  //normalizing and no normalizing
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    user_coins[0][i]->no_normalization();
    id = user_coins[0][i]->getId_User();
    uc_index[0][id] = i;
  }
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    user_coins[0][i]->normalization();
  }

}

//insert in LSH table
void insert_lsh_table(HashTable ** table, GFunctions ** fs, vector<User_coins*> * user_coins, map<string, vector<double> *> * map_vector, int dimension, int L, int k){
  long long int g;
  string id;
  int i, j, tablesz, in, n;

  n = user_coins->size();
  //cosine metric
  tablesz = pow(2.0, double(k));
  for(i=0; i < L; i++)
    fs[i] = new GCosine(k, dimension);

  for(i=0; i < L; i++){
    table[i] = new HashTable(tablesz);
    for (j = 0; j < n; j++){
      g = fs[i]->getNum(*(user_coins[0][j]->getN_Coins()));
      in = fs[i]->getBucket_Num(g);

      id = user_coins[0][j]->getId_User();
      map_vector[0][id] = user_coins[0][j]->getV_Coins();
      table[i]->add_item(in, id, g, user_coins[0][j]->getN_Coins());
    }
  }
}

//search neighbours into tables
void Search_lsh_tables(vector<User_coins *> * uc, int index, map <string, double> *map_index, HashTable ** table, GFunctions ** g, int L){
  long long int num;
  //to avoid duplicates
  list<string> key_list;
  list<HashNode<TP>*> * temp;

  vector<TP> * q = uc[0][index]->getN_Coins();

  string id = uc[0][index]->getId_User();

  for(int i=0; i < L; i++){
    num = g[i]->getNum(*q);
    temp = table[i]->return_bucket(g[i]->getBucket_Num(num));
    //if(temp->size() > 3*L) break; //trick stopping sooner the algorithm
    for (std::list< HashNode<TP>* >::iterator it = temp->begin(); it != temp->end(); ++it){
      string k = (*it)->getKey();
      long long int g = (*it)->getG();
      if(k == id) continue;
      if(g != num) continue;
      vector <TP> *p = (*it)->getValue();
      list<string>::iterator result1 = find(key_list.begin(), key_list.end(), k);
      if (result1 == key_list.end()) {
        key_list.push_back(k);
        //similarity
        map_index[0][k]=cosine_distance(*q, *p);
      }
    }
  }
}

//finding P best neighbours
void P_best_neighbours(map <string, double> *map_index , int& p){
  map<string,double>::iterator it;
  vector< sort_map > v;
  vector< sort_map >::iterator itv;
  sort_map sm;

  for (it = map_index->begin(); it != map_index->end(); ++it){
    sm.key = (*it).first; sm.val = (*it).second;
    v.push_back(sm);
  }

  sort(v.begin(),v.end(),Sort_by);
  
  map_index->clear();
  
  int counter = 0;
  for (itv = v.begin(); itv != v.end(); ++itv){
    map_index[0][(*itv).key] = (*itv).val ;
    counter++;
    if(counter == p) break;
  }
}

//final recommended coins for the user
void final_coins(User_coins * user_coins, map<string, vector<double> *> * map_vector, map <string, double> *map_index, int choice, bool vrtl){
  //compute neighbor coins
  vector<int> * vec = user_coins->getUnknown_Coins();
  vector<double> * v_coin = user_coins->getV_Coins();
  vector<double> v_d;
  double sum, acc=0.0;
  for(vector<int>::const_iterator it = vec->begin(); it != vec->end(); ++it){
    sum = 0.0; acc = 0.0;
    for(map<string, double>::const_iterator iter = map_index->begin(); iter != map_index->end(); ++iter){
      v_coin = map_vector[0][iter->first];
      if(choice){
        //similarity-euclidean
        sum += v_coin[0][*it]*(1/(1+iter->second));
        acc += (1/(1+iter->second));
      }else{
        //similarity-cosine
        sum += v_coin[0][*it]*(1-iter->second);
        acc += (1-iter->second);
      }
    }
    if((acc != 0.0) && (!isnan(sum) && !isnan(acc)))
      v_d.push_back(sum*(1/acc));
    else
      v_d.push_back(0);
  }
  
  //sorting
  map<string,double>::iterator it;
  vector< sort_map > v;
  vector< sort_map >::iterator itv;
  sort_map sm;

  map<string, double> * help = new map<string, double>();
  
  for(vector<double>::size_type i = 0; i < v_d.size(); i++){
    help[0][to_string(vec[0][i])] = v_d[i];
  }

  for (it = help->begin(); it != help->end(); ++it){
    sm.key = (*it).first; sm.val = (*it).second;
    v.push_back(sm);
  }
  
  sort(v.begin(),v.end(),Sort_by_up);
  
  int stop = 0;
  if(vrtl)
    stop = S_V;
  else
    stop = S_N;
  
  //storing the (S_N or S_V) indexes of the best recommended coins
  int counter = 0;
  for (itv = v.begin(); itv != v.end(); ++itv){
    user_coins->push_back_final(stoi(itv->key));
    counter++;
    if(counter == stop) break;
  }
  
  help->clear();
  delete help;
}