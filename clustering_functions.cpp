#include "functions.h"
#include "algorithmic_functions.h"

//clustering algorithm
void clustering_algorithm(vector<User_coins*> * user_coins, int dimension){
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
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
    final_coins(user_coins[0][i], map_vector, map_index, 1, false);
    map_index->clear();
  }

  //deletion
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

}

//clustering algorithm for virtual users
void clustering_algorithm_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, int dimension){
  vector<string> * in_ids = new vector<string>();
  vector< vector<TP> > *points = new vector< vector<TP> >() ;
  map<string, vector<double> *> * map_vector = new map<string, vector<double> *>();
  map <string, double> *map_index = new map<string, double>();
  int n = user_coins->size() + user_coins_v->size();
  //make dots-points-in_ids-mapvector
  create_struct_for_clustering_virtual(user_coins, user_coins_v, in_ids, points, map_vector);
  Dots * dots = new Dots(points, in_ids, n);

  simple_clustering(dots, points, in_ids, dimension, n);
  for ( vector<User_coins*>::size_type i = 0; i < user_coins->size(); i++ ){
    find_neighbours_from_clustering_virtual(user_coins[0][i], dots, map_index);
    //compute neighbor coins
    final_coins(user_coins[0][i], map_vector, map_index, 1, true);
    map_index->clear();
  }

  //deletion
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
}

//clustering for creating virtual users
void virtualclustering(vector< vector<TP> > *points, vector<string> * in_ids, Tweets * tweets, map<int, Tweets* > * v_users){
  vector < vector<TP> > *centroids = new vector< vector<TP> >();
  vector<string> * centroids_ids = new vector<string>();
  //euclidean metric for clustering
  int metric = 0;
  int num_clusters=NUM_V;
  int n = points->size();
  int dimension = points[0][0].size();
  Dots * dots= new Dots(points, in_ids, n);

  initialization_random(points, in_ids, centroids, centroids_ids, num_clusters, n);

  //assignment
  int counter = n;
  while( counter > 0){
    counter=0;
    Lloyds_assignment(dots, points, centroids, counter, n, metric);
    update_kmeans(dots, centroids, num_clusters, dimension);
    //update_kmedoid(dots, centroids, centroids_ids, num_clusters, metric);
  }
  //find best clustering with silhouette
  //double *sil = new double[num_clusters+1];
  //silhouette(centroids, dots, sil, metric, num_clusters, n);
  
  create_virtual_users(dots, v_users, tweets, num_clusters);

  //deletion
  //delete[] sil;
    
  centroids_ids->clear();
  delete centroids_ids;

  centroids->clear();
  delete centroids;

  delete dots;
}

//create the necessary structs(points, ids) for clustering
void create_struct_for_clustering(vector<User_coins*> * user_coins, vector<string> * in_ids, vector<vector<TP> > *points, map<string, vector<double> *> * map_vector){
  string id;
  long unsigned int size = user_coins->size();
  for ( vector<User_coins*>::size_type i = 0; i < size; i++ ){
    points->push_back(*(user_coins[0][i]->getN_Coins()));
    id = user_coins[0][i]->getId_User();
    in_ids->push_back(id);
    map_vector[0][id]= user_coins[0][i]->getV_Coins();
  }
}

//random initialazation of the centroids that belong to dataset (2nd Project)
void initialization_random(vector< vector<TP> > *points, vector<string> * in_ids, vector < vector<TP> > *centroids, vector<string> * centroids_ids, int& num_clusters, int& n){
	vector <int> num_centroids;
	vector<int>::iterator iter;

  while((int) num_centroids.size() != num_clusters){  
    //n == total number of points
    int rand_num = rand() % n;
    iter = find (num_centroids.begin(), num_centroids.end(), rand_num);
    //all the random values that will be produced must be different
    if (iter == num_centroids.end())
      num_centroids.push_back(rand_num);
	}

  for(int i=0; i < num_clusters; i++){
    centroids->push_back(points[0][num_centroids.at(i)]);
    centroids_ids->push_back(in_ids[0][num_centroids.at(i)]);
  }
}

//function that implements the simple algorithm Lloyd's assignment (2nd Project)
void Lloyds_assignment(Dots *dots, vector< vector<TP> > *points, vector < vector<TP> > *centroids, int& counter, int n, int metric){
  int index = -1;
  for(int i=0; i<n; i++){
    min_distance(points[0][i], centroids, metric, &index);
    //check if the point belongs to the same cluster as before (termination condition)
    if(dots->returnIndex(i) != index)
      counter++;
    dots->setIndexforPoint(i, index);
  }
}

//update the centers with the mean of each custer (2nd Project)
void update_kmeans(Dots *dots, vector < vector<TP> > *centroids, int num_clusters, int dimension){
  vector <TP> new_centroid;
  vector<TP> *temp;
  int size_cluster = 0;
  double mean = 0.0, sum = 0.0;

  for(int i=0; i<num_clusters; i++){
    for(int j=0; j<dimension; j++){
      sum = 0.0;
      size_cluster = 0;
      for(int k = 0; k < dots->getSize(); k++){
        if(i == dots->returnIndex(k)){
          temp = dots->returnPoint(k);
          sum += temp[0][j];
          size_cluster++;
        }
      }
      if(size_cluster == 0)
        mean=0;
      else
        mean = sum/size_cluster;
      new_centroid.push_back(mean);
    }
    centroids[0][i] = new_centroid;
    new_centroid.clear();
  }
}

//update the centers with the medoid of the cluster (2nd Project)
void update_kmedoid(Dots * dots, vector < vector<TP> > *centroids, vector<string> * centroids_ids, int num_clusters, int m){
  vector <TP> *new_centroid;
  string new_id;
  vector<TP> *temp1, *temp2;
  vector<double> distances;
  double min_sum = double(INT_MAX);
  double dist = 0.0, sum = 0.0;

  for(int i=0; i < num_clusters; i++){
    min_sum = double(INT_MAX);
    for(int j=0; j < dots->getSize(); j++){
      sum=0.0;
      if(i == dots->returnIndex(j)){
        if(centroids_ids[0][i] != dots->returnIdPoint(j)){
          temp1 = dots->returnPoint(j);
          for(int k=0; k < dots->getSize(); k++){
            if(k != j){
              if(i == dots->returnIndex(k)){
                temp2 = dots->returnPoint(k);
                if(m)
                  dist=cosine_distance(*temp1, *temp2);
                else
                  dist=euclidean_distance(*temp1, *temp2);
                distances.push_back(dist);
              }
            }
          }
          sum = accumulate(distances.begin(), distances.end(), 0.0);
        }
        if(distances.size() == 0){
            new_centroid = dots->returnPoint(j);
            new_id = dots->returnIdPoint(j);
        }else{
          if(sum < min_sum){
            min_sum = sum;
            new_centroid = dots->returnPoint(j);
            new_id = dots->returnIdPoint(j);
          }
        }
        distances.clear();
      }
    }
    centroids[0][i] = *new_centroid;
    centroids_ids[0][i] = new_id;
  }
}

//computation of silhouette (2nd Project)
void silhouette(vector < vector<TP> > *centroids, Dots * dots, double* sil, int metric, int num_clusters, int num){
  vector<TP> *temp, *temp1, *temp2;
  int index, neigh_index;
  int size_a = 0, size_b=0;
  double dist, a = 0.0, b=0.0, s=0.0;

  //refresh the array of silhouettes
  for(int i=0; i<num_clusters +1; i++){
    sil[i]=0.0;
  }
  //computation of silhouette
  for(int i=0; i<num; i++){
    temp = dots->returnPoint(i);
    index = dots->returnIndex(i);
    vector<vector<TP> > elem(centroids[0]);
    elem.erase(elem.begin()+ index);
    min_distance(centroids[0][index], &elem, metric, &neigh_index);
    if(index <= neigh_index)
      neigh_index = neigh_index + 1;
    size_a=0; size_b=0; a=0.0; b=0.0;
    for(int j=0; j<num; j++){
      if(i != j){
        if(dots->returnIndex(j) == index){
          temp1 = dots->returnPoint(j);
          if(!metric)
            dist = euclidean_distance(*temp, *temp1);
          else
            dist = cosine_distance(*temp, *temp1);
          a += dist;
          size_a++;
        }
      }
      if(dots->returnIndex(j) == neigh_index){
        temp2 = dots->returnPoint(j);
        if(!metric)
          dist = euclidean_distance(*temp, *temp2);
        else
          dist = cosine_distance(*temp, *temp2);
        b += dist;
        size_b++;
      }
    }
    //for size_a == 0 or size_b == 0 the result is NaN and goes to else that is 0
    a = a/size_a;
    b = b/size_b;

    if(a < b)
      s = 1 - a/b;
    else if(a > b)
      s = b/a - 1;
    else
      s = 0.0;

    sil[index] += s;
    sil[num_clusters] += s;
  }
}

//finding neighbours tha belong to the same cluster
void find_neighbours_from_clustering(vector<User_coins *> * user_coin, int ind, Dots* dots, map <string, double> *map_index){
  string id = user_coin[0][ind]->getId_User();
  
  int index = dots->returnIndexbyKey(id);
 
  vector<TP> * temp, *temp2;
  temp = user_coin[0][ind]->getN_Coins();

  for(int i=0; i< dots->getSize(); i++){
    if(dots->returnIdPoint(i) != id){
      if(dots->returnIndex(i) == index){
      	temp2 = dots->returnPoint(i);
        map_index[0][dots->returnIdPoint(i)]=euclidean_distance(*temp, *temp2);
      }
    }
  }
}

//a simple clustering merhod
void simple_clustering(Dots * dots, vector< vector<TP> > *points, vector<string> * in_ids, int dimension, int n){
    vector < vector<TP> > *centroids = new vector< vector<TP> >();
  	vector<string> * centroids_ids = new vector<string>();
    //euclidean metric
    int metric = 0;
    //the preffered size of cluster according to the excercise
  	int num_clusters=n/P;
  	
  	initialization_random(points, in_ids, centroids, centroids_ids, num_clusters, n);
  	//assignment
  	int counter = n;
  	while( counter > 0){
    	counter=0;
    	Lloyds_assignment(dots, points, centroids, counter, n, metric);
    	update_kmeans(dots, centroids, num_clusters, dimension);
    	//update_kmedoid(dots, centroids, centroids_ids, num_clusters, metric);
  	}
  	//double *sil = new double[num_clusters+1];
  	//silhouette(centroids, dots, sil, metric, num_clusters, n);
  	//deletion
  	//delete[] sil;
  	
  	centroids_ids->clear();
  	delete centroids_ids;

  	centroids->clear();
  	delete centroids;
}

//create structs(points, ids) for clustering using virtual users
void create_struct_for_clustering_virtual(vector<User_coins*> * user_coins, vector<User_coins*> * user_coins_v, vector<string> * in_ids, vector<vector<TP> > *points, map<string, vector<double> *> * map_vector){
  string id;
  long unsigned int size = user_coins_v->size();
  for ( vector<User_coins*>::size_type i = 0; i < size; i++ ){
    points->push_back(*(user_coins_v[0][i]->getN_Coins()));
    id = user_coins_v[0][i]->getId_User();
    in_ids->push_back(id);
    map_vector[0][id]= user_coins_v[0][i]->getV_Coins();
  }
  size = user_coins->size();
  for ( vector<User_coins*>::size_type i = 0; i < size; i++ ){
    points->push_back(*(user_coins[0][i]->getN_Coins()));
    id = user_coins[0][i]->getId_User();
    in_ids->push_back(id);
  }
}

//find neighbours from the same cluster using virtual users
void find_neighbours_from_clustering_virtual( User_coins * user, Dots* dots, map <string, double> *map_index){
  string id = user->getId_User();
  int index = dots->returnIndexbyKey(id);
  
  vector<TP> * temp, *temp2;
  temp = user->getN_Coins();

  for(int i=0; i< dots->getSize(); i++){
    size_t found = dots->returnIdPoint(i).find("v");
    if (found != string::npos){
      if(dots->returnIndex(i) == index){
        temp2 = dots->returnPoint(i);
        map_index[0][dots->returnIdPoint(i)]=euclidean_distance(*temp, *temp2);
      }
    }
  }
}