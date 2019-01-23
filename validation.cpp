#include "functions.h"
#include "algorithmic_functions.h"

//function for validation algorithm 10-fold cross validation
void validation_function(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, map<string, int>* uc_index, string output_name, int dimension, int& p){
  ofstream myfile_out;
  vector<Validation_Pairs*> * pairs = new vector<Validation_Pairs*>();
  //the number of algorithms that will run
  int counter = 4;
  double mae = 0.0;
  //create set of pairs
  create_set_of_pairs(user_coins, pairs);
  
  myfile_out.open(output_name.c_str());
  while(counter > 0){
    //suffle
    random_shuffle ( pairs->begin(), pairs->end() );
    mae = 0.0;
    for(int f = 0; f < 10; f++){
      cout << "Fold " << f << endl;
      set_users(user_coins, pairs, uc_index, f);   
      // no and normalazation
      n_normalazation(user_coins);
      //virtual users
      n_normalazation(user_coins_v);
      
      if(counter > 2){
        //normal users
        if(counter % 2 == 0)
          //lsh-cosine
          mae += lsh_cosine_algorithm_validation(user_coins, pairs, uc_index, dimension, p, f);
        else
          //clustering
          mae += lsh_cosine_algorithm_virtual_validation(user_coins, user_coins_v, pairs, uc_index, dimension, p, f);
      }else{
        //virtual users
        if(counter % 2 == 0)
          //nn-lsh-cosine
          mae += clustering_algorithm_validation(user_coins, pairs, uc_index, dimension, f);
        else
          //clustering
          mae += clustering_algorithm_virtual_validation(user_coins, user_coins_v, pairs, uc_index, dimension, f);
      }

      for ( vector<User_coins*>::size_type i = 0; i < user_coins_v->size(); i++ )
        user_coins_v[0][i]->reset_coins();
      reset_users(user_coins, pairs, uc_index, f);
    }
    //print results
    if(counter > 2){
        if(counter % 2 == 0){
          cout << "End of Cosine LSH validation\n";
          myfile_out << "Cosine LSH Recommendation MAE: " << mae/10 << endl;
        }else{
          cout << "End of Cosine LSH (Virtual Users) validation\n";
          myfile_out << "Cosine LSH Recommendation MAE (Virtual Users): " << mae/10 << endl;
        }
      }else{
        if(counter % 2 == 0){
          cout << "End of Clustering validation\n";
          myfile_out << "Clustering Recommendation MAE: " << mae/10 << endl;
        }else{
          cout << "End of Clustering (Virtual Users) validation\n";
          myfile_out << "Clustering Recommendation MAE (Virtual Users): " << mae/10 << endl;
        }
      }
    counter--;
  }
  myfile_out.close();

  //deletion
  for(vector<Validation_Pairs*>:: iterator it = pairs->begin(); it!=pairs->end(); ++it){
    delete *it;
  }
  delete pairs;
  
}

//lsh-cosine algorithm for validation
double lsh_cosine_algorithm_validation(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int& p, int f){
  //default numbers
  int L=5, k=4;
  HashTable **table = new HashTable*[L]();
  GFunctions ** fs = new GFunctions*[L]();
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  map<string, vector<double> > * map_final = new map<string, vector<double> >();
  double mae = 0.0;
  insert_lsh_table(table, fs, user_coins, map_vector, dimension, L, k);

	for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    Search_lsh_tables(user_coins, i, map_index, table, fs, L);
    //find final results
    P_best_neighbours(map_index, p);
    //compute neighbor coins
    final_coins_validation(user_coins[0][i], map_vector, map_index, map_final, 0);
    map_index->clear();
  }
  mae = compute_mae(user_coins, pairs, uc_index, map_final, f);

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
  	
  map_final->clear();
  delete map_final;
  	
  return mae;
}

//lsh-cosine algorithm using virtual users for validation
double lsh_cosine_algorithm_virtual_validation(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int& p, int f){
	int L=5, k=4;
  HashTable **table = new HashTable*[L]();
  GFunctions ** fs = new GFunctions*[L]();
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  map<string, vector<double> > * map_final = new map<string, vector<double> >();
  double mae = 0.0;
  insert_lsh_table(table, fs, user_coins_v, map_vector, dimension, L, k);

  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    Search_lsh_tables(user_coins, i, map_index, table, fs, L);
    //find final results
    P_best_neighbours(map_index, p);
    //compute neighbor coins
    final_coins_validation(user_coins[0][i], map_vector, map_index, map_final, 0);
    map_index->clear();
  }
  mae = compute_mae(user_coins, pairs, uc_index, map_final, f);

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
  	
  map_final->clear();
  delete map_final;
  	
  return mae;
}

//clustering algorithm for validation
double clustering_algorithm_validation(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int f){
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  map<string, vector<double> > * map_final = new map<string, vector<double> >();
  double mae = 0.0;
  int n = user_coins->size();
  vector<string> * in_ids = new vector<string>();
  vector< vector<TP> > *points = new vector< vector<TP> >() ;
  //make dots-points-in_ids-mapvector
  create_struct_for_clustering(user_coins, in_ids, points, map_vector);
  Dots * dots = new Dots(points, in_ids, n);
  
  simple_clustering(dots, points, in_ids, dimension, n);

  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    find_neighbours_from_clustering(user_coins, i, dots, map_index);
    //compute neighbor coins
    final_coins_validation(user_coins[0][i], map_vector, map_index, map_final, 1);
    map_index->clear();
  }
  
  mae += compute_mae(user_coins, pairs, uc_index, map_final, f);

  //deletion
  map_final->clear();
  delete map_final;

  map_vector->clear();
  delete map_vector;

  map_index->clear();
  delete map_index;

  in_ids->clear();
  delete in_ids;

  n = points->size();
  for(int i=0; i< n; i++)
    points[0][i].clear();
  points->clear();
  delete points;

  delete dots;

  return mae;
}

//clustering algorithm using virtual users for validation
double clustering_algorithm_virtual_validation(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, int dimension, int f){
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  map<string, vector<double> > * map_final = new map<string, vector<double> >();
  double mae = 0.0;
  int n = user_coins->size() + user_coins_v->size();
  vector<string> * in_ids = new vector<string>();
  vector< vector<TP> > *points = new vector< vector<TP> >() ;
  //make dots-points-in_ids-mapvector
  create_struct_for_clustering_virtual(user_coins, user_coins_v, in_ids, points, map_vector);
  Dots *dots = new Dots(points, in_ids, n);

  simple_clustering(dots, points, in_ids, dimension, n);
  
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    find_neighbours_from_clustering_virtual(user_coins[0][i], dots, map_index);
    //final_coins
    final_coins_validation(user_coins[0][i], map_vector, map_index, map_final, 1);  
    map_index->clear();
  }
  mae = compute_mae(user_coins, pairs, uc_index, map_final, f);

  //deletion
  map_final->clear();
  delete map_final;

  map_vector->clear();
  delete map_vector;

  map_index->clear();
  delete map_index;

  in_ids->clear();
  delete in_ids;

  n = points->size();
  for(int i=0; i< n; i++)
    points[0][i].clear();
  points->clear();
  delete points;

  delete dots;

  return mae;
}

//create users with vector of coins for validation
void create_user_coins_validation(map<int, Tweets* > * users, vector< vector<string> > * coins, vector<User_coins*> * user_coins, map<string, int>* uc_index, bool vrtl){
  Tweets * t;
  double value;
  string id;
  int flag = 0;
  for(map<int, Tweets *>::iterator it = users->begin(); it != users->end(); ++it){
    t = it->second;
    User_coins * uc;
    if(vrtl){
      //v stands out for virtual user
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
      if(!isnan(value) && (value != 0))
        flag = 1;
    }
    if(((long unsigned int)uc->returnSize_of_Unknown_Coins() != coins->size()) && (flag == 1))
      user_coins->push_back(uc);
    else
      delete uc;
  }
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    id = user_coins[0][i]->getId_User();
    uc_index[0][id] = i;
  }
}

//create set of pairs(user,coin)
void create_set_of_pairs(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs){
  Validation_Pairs * p;
  vector<double> * v;
  vector<int> * knw;
  string id;
  int index = -1;
  for( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    id = user_coins[0][i]->getId_User();
    v = user_coins[0][i]->getCoins();
    knw = user_coins[0][i]->getKnown_Coins();
    for(vector<int> :: size_type j=0; j < knw->size(); j++){
      index = knw[0][j];
      p = new Validation_Pairs(id, index, v[0][index]);
      pairs->push_back(p);
    }
  }
}

//set users for validation and train set
void set_users(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs,  map<string, int>* uc_index,  int f){
  long unsigned int from = 0, to = 0;
  string id;
  int index = 0;
  int j = 0;
  if(f == 9){
    from = f*(pairs->size()/10);
    to = pairs->size() - to;
  }else{
    from = f*(pairs->size()/10);
    to = (f+1)*(pairs->size()/10);
  }
  vector<double> * v;
  vector<int> * u;
  vector<int> * k;
  for(vector<Validation_Pairs*>:: size_type i = from; i < to; i++){
    index = pairs[0][i]->getIndex_of_Coin();
    id = pairs[0][i]->getId_of_User();
    j = uc_index[0][id];
    v = user_coins[0][j]->getCoins();
    u = user_coins[0][j]->getUnknown_Coins();
    k = user_coins[0][j]->getKnown_Coins();
    //in case that user will have a vector full of NaN
    if(k->size() > 1){
      u->push_back(index);
      vector<int>::iterator position = find(k->begin(), k->end(), index);
      k->erase(position);
      //make unkwon coins
      v[0][index] = NAN;
    }else{
      pairs[0][i]->setSafe(false);
    }
  }
}

//normalizing and no normalizing vectors
void n_normalazation(vector<User_coins*> * user_coins){
	for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    user_coins[0][i]->no_normalization();
  }
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    user_coins[0][i]->normalization();
  }
}

//final coins for validation
void final_coins_validation(User_coins * user_coins, map<string, vector<double> *> * map_vector, map <string, double> *map_index, map<string , vector<double> > *map_final, int choice){
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
  //a map that holds the computed sentiment of the coins for the user
  map_final[0][user_coins->getId_User()]= v_d;
}

//computation of the mean absolute error
double compute_mae(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs, map<string, int>* uc_index, map<string , vector<double> > *map_final, int f){
  long unsigned int from = 0, to = 0;
  string id;
  int in=0;
  size_t index;
  double value = 0.0, mean = 0.0;
  int j = 0;
  int counter = 0;
  if(f == 9){
    from = f*(pairs->size()/10);
    to = pairs->size();
  }else{
    from = f*(pairs->size()/10);
    to = (f+1)*(pairs->size()/10);
  }
  vector<int> * vec;

  for(vector<Validation_Pairs*>:: size_type i = from; i < to; i++){
    //case that the pair has not been set and it doesnt have to be computed
    if(pairs[0][i]->getSafe()){
      in = pairs[0][i]->getIndex_of_Coin();
      id = pairs[0][i]->getId_of_User();
      value = pairs[0][i]->getTrue_Value();
      j = uc_index[0][id];
      vec = user_coins[0][j]->getUnknown_Coins();
      index = find(vec->begin(), vec->end(), in) - vec->begin();
      if(!isnan(map_final[0][id][index]))
        mean += abs(map_final[0][id][index] - value);
      else
        counter++;
    }else
      counter++;
  }
  return mean/((to-from)-counter);
}

//reset the users so they can be reused
void reset_users(vector<User_coins*> * user_coins, vector<Validation_Pairs*> * pairs,  map<string, int>* uc_index, int f){
  long unsigned int from = 0, to = 0;
  string id;
  int index = 0;
  double value = 0.0;
  int j = 0;
  if(f == 9){
    from = f*(pairs->size()/10);
    to = pairs->size() - to;
  }else{
    from = f*(pairs->size()/10);
    to = (f+1)*(pairs->size()/10);
  }
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    user_coins[0][i]->reset_coins();
  }
  vector<double> * v;
  vector<int> * u;
  vector<int> * k;
  for(vector<Validation_Pairs*>:: size_type i = from; i < to; i++){
    index = pairs[0][i]->getIndex_of_Coin();
    id = pairs[0][i]->getId_of_User();
    value = pairs[0][i]->getTrue_Value();
    j = uc_index[0][id];
    v = user_coins[0][j]->getCoins();
    u = user_coins[0][j]->getUnknown_Coins();
    k = user_coins[0][j]->getKnown_Coins();
    if(pairs[0][i]->getSafe()){
      k->push_back(index);
      vector<int>::iterator position = find(u->begin(), u->end(), index);
      u->erase(position);
      v[0][index] = value;
    }
  }
}