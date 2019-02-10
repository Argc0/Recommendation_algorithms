# Recommendation Algorithms    
The program is an implementation of **_Collaborative Recommendation Techniques_** for **cryptocurrencies(bitcoins)**. The program implements two _Recommendation Algorithms_ for the following suggestion problems: A. 5 "best" cryptocurrencies for real users and B. 2 "best" cryptocurrencies for virtual users. The first algorithm is **_Cosine LSH Recommendation_** that finds P nearest neighbors by using **_Cosine-LSH search_**. The second one is **_Clustering Recommendantion_** that compiles users into **k** clusters. Both algorithms are evaluated based on the **_Absolute Error Average_** by the **10-fold** **_Cross-Validation_** (each case ,A or B, is computed differently).

**_How to run(using terminal):_**  
**Recommendation:**  
./recommendation -d \<input file> -o \<output file> -t \<dataset file (2nd Project)> -l \<lexicon file> -c \<coins file>  
**Validation:**  
./recommendation -d \<input file> -o \<output file> -t \<dataset file (2nd Project)> -l \<lexicon file> -c \<coins file> -validate

