#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "functions.h"
#include "algorithmic_functions.h"


TEST_CASE( "Tests for Euclidean and Cosine distance", "[single-file]" ) {
	vector<TP> p,v;
	int n=100;
	for(int i=0; i < n; i++){
		p.push_back(i);
		v.push_back(i);
	}
	REQUIRE(euclidean_distance(p, v) == 0);
	//mporei na ypar3ei apokopi merikes fores
	REQUIRE(int(cosine_distance(p, v)) == 0);
	v.clear();
	p.clear();
	for(int i=0; i < n; i++){
		p.push_back(NAN);
		v.push_back(i);
	}
	REQUIRE(isnan(euclidean_distance(p, v)));
	REQUIRE(isnan(cosine_distance(p, v)));
	v.clear();
	p.clear();
	for(int i=0; i < n; i++){
		p.push_back(0);
		v.push_back(0);
	}
	REQUIRE(euclidean_distance(p, v) == 0);
	REQUIRE(isnan(cosine_distance(p, v)));
}


TEST_CASE( "Tests for the function that finds P best neighbours", "[single-file]" ) {
	map<string, double> map_index;
	int p = 5, n=100;
	
	for(int i=0; i<n; i++)
		map_index[to_string(i)]=double(i);
	P_best_neighbours(&map_index, p);
    REQUIRE( map_index.size() == p );
    
    map_index.clear();
    
    for(int i=n; i>0; i--)
		map_index[to_string(i)]=double(i);
    P_best_neighbours(&map_index, p);
    REQUIRE( map_index.size() == p );
    
    map_index.clear();
    
    p=n+1;
    for(int i=n; i>0; i--)
		map_index[to_string(i)]=double(i);
    P_best_neighbours(&map_index, p);
    REQUIRE( map_index.size() == n );
    
    map_index.clear();
    
    P_best_neighbours(&map_index, p);
    REQUIRE( map_index.size() == 0 );
}