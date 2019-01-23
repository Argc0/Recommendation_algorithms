#include "functions.h"
#include "algorithmic_functions.h"

int main(int argc, char *argv[]) {
	string input_name, output_name, lexicon_name, coins_name, dataset_name;
  ofstream myfile_out;
  bool validate = false;
  int choice = 0, p = P, dimension;
  clock_t begin_, end_;
  double elapsed_secs;

  map<string,double> *lexicon_map = new map<string, double>();
  vector< vector<string> > * coins = new vector< vector<string> >();
  map<int, Tweets* > * users = new map<int, Tweets* >();
  map<int, Tweets* > * v_users = new map<int, Tweets* >();
  vector<string> * in_ids = new vector<string>();
  vector< vector<TP> > *points = new vector< vector<TP> >() ;
  Tweets * tweets = new Tweets();
  vector<User_coins*> * user_coins = new vector<User_coins*>();
  vector<User_coins*> * user_coins_v = new vector<User_coins*>();
  //a map that store the indexes of the users_coins vectors
  map<string, int>* uc_index = new map<string, int>();

	srand((unsigned)time(0));

  //get arguments
	get_args(argc, argv, input_name, output_name, lexicon_name, coins_name, dataset_name, validate);

	cout << "The input file is: " << input_name << endl << "The output file is: " << output_name << endl << "Validate is: " << validate << endl;
  cout << "The lexicon file is: " << lexicon_name << endl << "The coins file is: " << coins_name << endl << "The dataset file is: " << dataset_name << endl;
  
  read_lexicon_file(lexicon_name, lexicon_map);

  read_coins_file(coins_name, coins);

  read_dataset_file(dataset_name, in_ids, points);

  read_input_file(input_name, users, tweets, p);

  tweets->compute_scores(lexicon_map);

  //virtual users clustering
  virtualclustering(points, in_ids, tweets, v_users);

  if(!validate){
    /*-----------------------------------------------LSH-Cosine----------------------------------------------------*/
    //virtual and simple users
    create_user_coins(v_users, coins, user_coins_v, uc_index, true);
    create_user_coins(users, coins, user_coins, uc_index, false);
    dimension = coins->size();
  
    myfile_out.open(output_name.c_str());

    //nn-lsh-cosine
    choice = 0;
    begin_= clock();
    cout << "LSH-Cosine Algorithm\n";
    lsh_cosine_algorithm(user_coins, dimension, p);
    end_= clock();
    elapsed_secs = double(end_ - begin_) / CLOCKS_PER_SEC;
    //print results
    print_results(user_coins, coins, elapsed_secs, choice, myfile_out);
  
  
    for(vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++)
      user_coins[0][i]->clear_final();

    //nn-lsh-cosine-virtual
    begin_ = clock();
    cout << "LSH-Cosine (Virtual Users) Algorithm\n";
    lsh_cosine_algorithm_virtual(user_coins, user_coins_v, dimension, p);
    end_= clock();
    elapsed_secs = double(end_ - begin_) / CLOCKS_PER_SEC;
    //print results
    myfile_out << "Virtual Users-";
    print_results(user_coins, coins, elapsed_secs, choice, myfile_out);

    for(vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++)
      user_coins[0][i]->clear_final();
    /*-------------------------------------------------Clustering------------------------------------------------------*/
    //clustering
    choice = 1;
    begin_= clock();
    cout << "Clustering Algorithm\n";
    clustering_algorithm(user_coins, dimension);
    end_= clock();
    elapsed_secs = double(end_ - begin_) / CLOCKS_PER_SEC;
    //print results
    print_results(user_coins, coins, elapsed_secs, choice, myfile_out);
  
    for(vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++)
      user_coins[0][i]->clear_final();

    //clustering-virtual
    begin_ = clock();
    cout << "Clustering (Virtual Users) Algorithm\n";
    clustering_algorithm_virtual(user_coins, user_coins_v, dimension);
    end_= clock();
    elapsed_secs = double(end_ - begin_) / CLOCKS_PER_SEC;
    //print results
    myfile_out << "Virtual Users-";
    print_results(user_coins, coins, elapsed_secs, choice, myfile_out);
  
    myfile_out.close();
  
  }else{ /*-------------------------------------------Validation--------------------------------------------------*/
    //create simple users and virtual users
    create_user_coins_validation(users, coins, user_coins, uc_index, false);
    create_user_coins_validation(v_users, coins, user_coins_v, uc_index, true);
    dimension = coins->size();
    
    cout << "Validation\n";
    validation_function(user_coins, user_coins_v, uc_index, output_name, dimension, p);
  }
  //deletion
  deletion(lexicon_map, coins, users, v_users, in_ids, points, tweets, user_coins, user_coins_v, uc_index);
  
  cout << "End!\n";
  return 0;
}