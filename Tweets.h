#ifndef __TWEETS__
#define __TWEETS__

//classes helping the recommendation algorithms

//class for storing a tweet
class Tweet{
  
  private:
    string id;
    double score;
    vector<string> *text;
	
  public:
    Tweet(string id, vector<string>& v){
      Tweet::id = id;
      //[-1,1]
      score = double(A);
      text = new vector<string>(v);
    }
		~Tweet(){
		  text->clear();
		  delete text;
    }
    void setId(string id){
      Tweet::id = id;
    }
    void setText(vector<string> * text){
      Tweet::text = text;
    }
    void setScore(double score){
      Tweet::score = score;
    }
    string getId(){
      return id;
    }
    vector<string> * getText(){
      return text;
    }
    double getScore(){
      return score;
    }
    //computation of score for the tweet
    void score_value(map<string, double> *lexicon_map){
      double sum_score = 0.0, total_score;
      map<string, double>::iterator iter;
      string var;
      for ( std::vector<string>::size_type i = 0; i < text->size(); i++ ){
        var = text[0][i];
        iter = lexicon_map->find(var);
        if (iter != lexicon_map->end())
          sum_score += lexicon_map[0][var];
        else
          sum_score += 0.0;
      }
      if(sum_score == 0.0)
        total_score = 0.0;
      else
        total_score = sum_score / sqrt((sum_score*sum_score + A));
      this->setScore(total_score);
    }
    //finds if a coin is inside in a tweet
    bool coin_in_tweet(vector<string>& coins){
      std::vector<string>::iterator it;
      for(vector<string>::size_type i = 0; i < coins.size(); i++ ){
        it = find (text->begin(), text->end(), coins[i]);
        if (it != text->end())
          return true;
        }
      return false;
    }
    void print_tweet(){
      cout << id << " ";
      for ( vector<string>::size_type i = 0; i < text->size(); i++ )
        cout << text[0][i] << " ";
      cout << endl;
    }
};

//class for storing tweets for a user
class Tweets {
  
  private:
    vector<Tweet*> * v;
    map<string, Tweet*> * m;
  
  public:
    Tweets(){
      v = new vector<Tweet*>();
      m = new map<string, Tweet*>();
    }
    ~Tweets(){
      v->clear();
      m->clear();
      delete v;
      delete m;
    }
    vector<Tweet*> * getV(){
      return v;
    }
    int getSize(){
      return v->size();
    }
    void add_tweet(Tweet * tweet){
      v->push_back(tweet);
      string t_id = tweet->getId();
      m[0][t_id] = tweet;
    }
    void compute_scores(map<string, double> *lexicon_map){
      for ( vector<Tweet>::size_type i = 0; i < v->size(); i++ )
        v[0][i]->score_value(lexicon_map);
    }
    Tweet * getTweet(string id){
      return m[0][id];
    }
    //finding if a coin exists inside the tweets
    double find_coins(vector<string>& coins){
      double score = 0.0;
      int flag = 0;
      for ( std::vector<Tweet*>::size_type i = 0; i < v->size(); i++ ){
        if (v[0][i]->coin_in_tweet(coins)){
          score += v[0][i]->getScore();
          flag = 1;
        }
      }
      if(!flag)
        score = NAN;
      return score;
    }
    void print(){
      for ( std::vector<Tweet*>::size_type i = 0; i < v->size(); i++ )
        v[0][i]->print_tweet();
		}
    void print_scores(int& n){
      for ( std::vector<Tweet*>::size_type i = 0; i < v->size(); i++ ){
        cout << v[0][i]->getScore() << endl;
        if(v[0][i]->getScore() == 0)
          n++;
      }
    }
};


//class for User's coins
class User_coins {
  
  private:
    string id_user;
    //original coin vector
    vector<double> * coins;
    //normalizing coin vector
    vector<double> * v_coins;
    //no normalazing coin vector
    vector<double> * n_coins;
    //vector of the unknown indexes of coins
    vector<int> * unknown_coins;
    //vector of the known indexes of coins
    vector<int> * known_coins;
    //final indexes for recommendation
    vector<int> * final;
  
  public:
    User_coins(string id){
      id_user = id;
      coins = new vector<double>();
      v_coins = new vector<double>();
      n_coins = new vector<double>();
      unknown_coins = new vector<int>();
      known_coins = new vector<int>();
      final = new vector<int>();
    }
    ~User_coins(){
      coins->clear();
      v_coins->clear();
      unknown_coins->clear();
      known_coins->clear();
      final->clear();
      n_coins->clear();
      delete coins;
      delete v_coins;
      delete unknown_coins;
      delete known_coins;
      delete final;
      delete n_coins;
    }
    void reset_coins(){
      n_coins->clear();
      v_coins->clear();
    }
    void clear_final(){
      final->clear();
    }
    void setId_User(string id_user){
      User_coins::id_user = id_user;
    }
    void setV_Coins(vector<double> * v_coins){
      User_coins::v_coins = v_coins;
    }
    void setCoins(vector<double> * coins){
      User_coins::coins = coins;
    }
    void setN_Coins(vector<double> * n_coins){
      User_coins::n_coins = n_coins;
    }
    void setUnknown_Coins(vector<int> * unknown_coins){
      User_coins::unknown_coins = unknown_coins;
    }
    void setKnown_Coins(vector<int> * known_coins){
      User_coins::known_coins = known_coins;
    }
    void setFinal(vector<int> * final){
      User_coins::final = final;
    }
    string getId_User(){
      return id_user;
    }
    //returns the normalized vector of coins
    vector<double> * getV_Coins(){
      return v_coins;
    }
    vector<double> * getCoins(){
      return coins;
    }
    //returns the no normalized vector of coins
    vector<double> * getN_Coins(){
      return n_coins;
    }
    vector<int> * getUnknown_Coins(){
      return unknown_coins;
    }
     vector<int> * getKnown_Coins(){
      return known_coins;
    }
    vector<int> * getFinal(){
      return final;
    }
    int returnSize_of_Unknown_Coins(){
      return unknown_coins->size();
    }
    int returnSize_of_Known_Coins(){
      return known_coins->size();
    }
    void add_value(double value){
      coins->push_back(value);
    }
    void add_index_un(int index){
      unknown_coins->push_back(index);
    }
    void add_index(int index){
      known_coins->push_back(index);
    }
    void push_back_final(int key){
      final->push_back(key);
    }
    //no normalizing the user's vector of coins
    void no_normalization(){
      double mean = 0.0, total_mean = 0.0;
      int num = 0;
      for ( vector<double>::size_type i = 0; i < coins->size(); i++ ){
        if(!isnan(coins[0][i])){
          mean += coins[0][i];
          num++;
        }
      }
      if(num != 0)
        total_mean = mean/num;
      for ( vector<double>::size_type i = 0; i < coins->size(); i++ ){
        if(!isnan(coins[0][i]))
          n_coins->push_back(coins[0][i]);
        else
          n_coins->push_back(total_mean);
      }
    }
    //normalizing the user's vector of coins
    void normalization(){
      double mean = 0.0, total_mean;
      int num = 0;
      for ( vector<double>::size_type i = 0; i < coins->size(); i++ ){
        if(!isnan(coins[0][i])){
          mean += coins[0][i];
          num++;
        }
      }
      total_mean = mean/num;
      for ( vector<double>::size_type i = 0; i < coins->size(); i++ ){
        if(!isnan(coins[0][i]))
          v_coins->push_back(coins[0][i] - total_mean);
        else
          v_coins->push_back(0.0);
      }
    }
    void print(){
      for ( vector<double>::size_type i = 0; i < coins->size(); i++ ){
        cout << coins[0][i] << " ";
      }
      cout << endl;
      for ( vector<int>::size_type i = 0; i < unknown_coins->size(); i++ ){
        cout << unknown_coins[0][i] << "-";
      }
      cout << endl;
    }
};

//class for pairs (User, Coin) for validation algorithm
class Validation_Pairs{
  
  private:
    string id_of_user;
    int index_of_coin;
    double true_value;
    //variable to see if a pair is safe to be in validation set
    bool safe;
  
  public:
    Validation_Pairs(string id, int index, double value){
      Validation_Pairs:: id_of_user = id;
      Validation_Pairs::index_of_coin = index;
      Validation_Pairs::true_value = value;
      Validation_Pairs::safe = true;
    }
    void setId_of_User(string id){
      Validation_Pairs::id_of_user = id;
    }
    void setIndex_of_Coins(int index){
      Validation_Pairs::index_of_coin = index;
    }
    void setTrue_Value(double value){
      Validation_Pairs::true_value = value;
    }
    void setSafe(bool safe){
      Validation_Pairs::safe = safe;
    }
    string getId_of_User(){
      return id_of_user;
    }
    int getIndex_of_Coin(){
      return index_of_coin;
    }
    double getTrue_Value(){
      return true_value;
    }
    bool getSafe(){
      return safe;
    }
    void print(){
      cout << "Id: " << id_of_user << " ";
      cout << "Index: " << index_of_coin << " ";
      cout << "True value: " << true_value << endl;
    }
};
#endif