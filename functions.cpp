#include "functions.h"
#include "algorithmic_functions.h"

//function that returns the arguments from the coomand line
void get_args( int argc, char** argv, string& input_name, string& output_name, string& lexicon_name, string& coins_name, string& dataset_name, bool& validate){
	int i, j;

	if (argc > 1) {

		if(!(argc == 11 || argc == 12)){
			cout << "The arguments that given are not right!\n";
			exit(-1);
		}
    
    for (i = 1; i <= argc-1; i += 2) {

			// Check if argument is given again
			for( j = i+2; j <= argc-1; j += 2 ) {

				if( !strcmp(argv[i], argv[j]) ) {
					cout << "Error: Argument given again.\nPlease check README for more info.\n";
					exit(-1);
				}
			}
      
			// Get arguments
			if( !strcmp(argv[i], "-d") ) {
				input_name=argv[i+1];
			} else if( !strcmp(argv[i], "-o") ) {
				output_name = argv[i+1];
			} else if( !strcmp(argv[i], "-validate") ) {
				validate = true;
        i--;
			} else if( !strcmp(argv[i], "-l") ) {
        lexicon_name = argv[i+1];
      }else if( !strcmp(argv[i], "-c") ) {
        coins_name = argv[i+1];
      }else if( !strcmp(argv[i], "-t") ) {
        dataset_name = argv[i+1]; 
      }else {
				cout << "Error: Wrong argument given.\nPlease check README for more info.\n";
				exit(-1);
			}
		}
	} else {
		cout << "Error: Arguments are missing! Please check README for more info\n";
	}
	return;
}

//internal product of vectors
double internal_product(vector<TP> &p, vector<double> &v){
  int size = p.size();
  double sum = 0.0;
  
  for(int i = 0; i < size; ++i) {
    sum += p[i]*v[i];
  }
  return sum;
}

//euclidean distance of vectors
double euclidean_distance(vector<TP> &p, vector<TP> &v){
  int size = p.size();
  double d = 0.0;

  for(int i = 0; i < size; ++i) {
    //(y1-x1)^2
    d += (v[i] - p[i])*(v[i]-p[i]);
  }
  return sqrt(d);
}


//cosine distance of vectors
double cosine_distance(vector<TP> &x, vector<TP> &y){
  int size = x.size();
  double d = 0.0, in = 0, a = 0, b = 0;

  //internal_product
  for(int i = 0; i < size; ++i) {
    in += x[i]*y[i];
  }
  for(int i = 0; i < size; ++i) {
    a += x[i]*x[i];
    b += y[i] *y[i]; 
  }

  d = in/(sqrt(a)*sqrt(b));
  return 1 - d;
}

//reading parameters of the vader_lexicon file
void read_lexicon_file(string lexicon_filename, map<string,double> *lexicon_map){
	ifstream myfile_lex;
	string line, id, value;
	istringstream iss;

	myfile_lex.open(lexicon_filename.c_str());

	if (myfile_lex.is_open()){

		while ( getline (myfile_lex,line, '\t') ){

			id = line;
			getline (myfile_lex,line);
			value = line;
      lexicon_map[0][id] = stod(value);
		}
		myfile_lex.close();
	}else {
		cout << "Unable to open lexicon file\n";
		exit(-1);
	}
}

//reading data from the coins file
void read_coins_file(string coins_filename, vector< vector<string> > * coins){
	ifstream myfile_coin;
	string line;
	istringstream iss;
  vector<string> inputs;

	myfile_coin.open(coins_filename.c_str());

  //read from coins file the coins
  if (myfile_coin.is_open()){
    while ( getline (myfile_coin,line) ){
      iss.clear();
      iss.str(line);

    	while ( getline (iss, line, '\t') ){
        inputs.push_back(line);
      }
      coins->push_back (inputs);
 
      inputs.clear();
    }
    myfile_coin.close();
  }else { 
    cout << "Unable to open coins file\n";
    exit(-1);
  }
}

//reading tweets and users
void read_input_file(string& input_name, map<int, Tweets*> * users, Tweets * tweets, int& p){
	ifstream myfile_in;
	
	string line;
	istringstream iss;
  istringstream iss1;
  vector<string> inputs;

	int id;
	string t_id;
  bool first_line = true;
  int flag = 0;

	myfile_in.open(input_name.c_str());

  //read from input file the points
  if (myfile_in.is_open()){
    while ( getline (myfile_in,line) ){
      if(first_line){
        if (line.find("P:") != std::string::npos)
            flag = 1;
        first_line = false;
      }
      if(flag){
        long unsigned int position;
        string num;
        position = line.find(":");
        num = line.substr (position + 1);
        p =  stoi(num);
        flag = 0;
      }else{
        iss.clear();
        iss.str(line);
    
        copy((istream_iterator<string>(iss)), istream_iterator<string>(), back_inserter(inputs));

        id = stoi(inputs[0]);
        t_id = inputs[1];
        inputs.erase (inputs.begin());
        inputs.erase(inputs.begin());
        map<int, Tweets*>::iterator it;

        Tweet *tw = new Tweet(t_id, inputs);
        tweets->add_tweet(tw);

        it = users->find(id);
        if (it != users->end()){
          users[0][id]->add_tweet(tw); 
        }else{
          Tweets *tws = new Tweets();
          tws->add_tweet(tw);
          users[0][id] = tws;
        }
        inputs.clear();
      }
    }
    myfile_in.close();
  }else { 
    cout << "Unable to open input file\n";
    exit(-1);
  }
}

//reading from dataset
void read_dataset_file(string dataset_filename, vector<string> * in_ids, vector< vector<TP> > *points){
  ifstream myfile_in;
  
  string line;
  istringstream iss;
  vector<TP> inputs;
    
  double it;
  string id;
  //variable for reading and storing the id of the point
  int flag = 1;

  myfile_in.open(dataset_filename.c_str());

  //read from input file the points
  if (myfile_in.is_open()){
    while ( getline (myfile_in,line) ){
      iss.clear();
      iss.str(line);

      flag=1;
      while (iss >> it){
        if(flag){
          ostringstream strs;
          strs << it;
          string str = strs.str();
          in_ids->push_back(str);
          flag = 0;
        }else{
          inputs.push_back(it);
        }
        if (iss.peek() == ',' || iss.peek() == ' ' || iss.peek() == '\t')
          iss.ignore();
      }

      points->push_back (inputs);
 
      inputs.clear();
    }
    myfile_in.close();
  }else { 
    cout << "Unable to open input file";
    exit(-1);
  }
}

//creation of virtual users by clustering
void create_virtual_users(Dots* dots, map<int, Tweets* > * users, Tweets * tweets, int num_clusters){
  Tweet * tw;
  for(int i=0; i<num_clusters; i++){
    for(int j=0; j<dots->getSize(); j++){
      if(dots->returnIndex(j) == i){
        tw = tweets->getTweet(dots->returnIdPoint(j));
        map<int, Tweets*>::iterator it;

        it = users->find(i);
        if (it != users->end()){
          users[0][i]->add_tweet(tw); 
        }else{
          Tweets *tws = new Tweets();
          tws->add_tweet(tw);
          users[0][i] = tws;
        }
      }
    }
  }
}

//sort by ascending order
bool Sort_by(const sort_map& a ,const sort_map& b){
  return a.val < b.val;
}

//sort by descending order
bool Sort_by_up(const sort_map& a ,const sort_map& b){
  return a.val > b.val;
}

//function that returns the distance of the nearest neighbour and the index of him
double min_distance(vector<TP> &q, vector< vector<TP> > * points, int metric, int* index){
  int n = points[0].size();
  double true_d = double(INT_MAX), d;
  if(metric){
    for(int x=0; x < n; x++){
            d = cosine_distance(q, points[0][x]);
            if( true_d > d){
              true_d = d;
              *index = x;
            }
        }
  }else{
    for(int x=0; x < n; x++){
            d = euclidean_distance(q, points[0][x]);
            if( true_d > d){
              true_d = d;
              *index = x;
            }
        }       
  }
  return true_d;
}

//Printing final results accordingly the choice of algorithm
void print_results(vector<User_coins*> * user_coins, vector< vector<string> > * coins, double secs, int choice, ofstream& myfile_out){
  if(choice)
    myfile_out << "Clustering\n";
  else
    myfile_out << "Cosine LSH\n";

  vector<int> * final;
  int index;
  vector<string> vs;
  for( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    myfile_out << user_coins[0][i]->getId_User() << " ";
    final = user_coins[0][i]->getFinal();
    for(vector<int>::const_iterator it = final->begin(); it != final->end(); ++it){
      index = *it;
      vs = coins->at(index);
      //h ola 0
      if(vs.size() >= 5){
        myfile_out << vs.at(4) << " ";
      }else{
        myfile_out << vs.at(0) << " ";
      }
    }
    myfile_out << endl;
  }

  myfile_out << "Execution Time: " << secs << endl;
  myfile_out << endl;
}

//deletion
void deletion(map<string,double> *lexicon_map, vector< vector<string> > * coins, map<int, Tweets* > * users, map<int, Tweets* > * v_users, vector<string> * in_ids, vector< vector<TP> > *points, Tweets * tweets, vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, map<string, int>* uc_index){
  int n = 0;

  in_ids->clear();
  delete in_ids;

  n = points->size();
  for(int i=0; i< n; i++)
    points[0][i].clear();
  points->clear();
  delete points;

  lexicon_map->clear();
  delete lexicon_map;

  n = coins->size();
  for(int i=0; i< n; i++)
    coins[0][i].clear();
  coins->clear();
  delete coins;

  vector<Tweet*> * tv= tweets->getV();
  for(vector<Tweet*>:: size_type j =0; j < tv->size(); j++){
    delete tv[0][j];
  }
  delete tweets;

  for(map<int, Tweets*>:: iterator it = users->begin(); it!=users->end(); ++it){
    delete it->second;
  }
  users->clear();
  delete users;

  for(map<int, Tweets*>:: iterator it = v_users->begin(); it!=v_users->end(); ++it){
    delete it->second;
  }
  v_users->clear();
  delete v_users;

  n = user_coins->size();
  for(int i=0; i< n; i++)
    delete user_coins[0][i];
  user_coins->clear();
  delete user_coins;

  n = user_coins_v->size();
  for(int i=0; i< n; i++)
    delete user_coins_v[0][i];
  user_coins_v->clear();
  delete user_coins_v;

  uc_index->clear();
  delete uc_index;
}