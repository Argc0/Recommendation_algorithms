recommendation:
	g++ --std=c++11 -Wall -g recommendation.cpp functions.cpp hash.cpp lsh_cosine_functions.cpp clustering_functions.cpp validation.cpp -o recommendation -lm

test:
	g++ --std=c++11 -Wall test_cases.cpp functions.cpp lsh_cosine_functions.cpp hash.cpp -o test_cases

# clean house
clean:
	rm -f recommendation.o functions.o hash.o lsh_cosine_functions.o clustering_functions.o validation.o recommendation test_cases.o test_cases

# do a bit of accounting
count:
	wc recommendation.cpp functions.cpp hash.cpp clustering_functions.cpp lsh_cosine_functions.cpp validation.cpp
