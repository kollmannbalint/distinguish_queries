#include <iostream>
#include <vector>
#include "trie.h"
#include "double_trie.h"

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int alphabetSize;
    std::cin >> alphabetSize;

    int n;
    std::cin >> n;
    std::vector<std::string> positive(n);

    for(int i = 0; i < n; i++) {
        std::cin>>positive[i];
        if(positive[i] == "$")
            positive[i] = "";
    }

    int m;
    std::cin>>m;
    std::vector<std::string> negative(m);

    for(int i = 0; i < m; i++) {
        std::cin>>negative[i];
        if(negative[i] == "$")
            negative[i] = "";
    }


    size_t cnt_symbols = 0;
    for(const auto& x : positive){
        cnt_symbols += x.size();
    }
    for(const auto& x : negative){
        cnt_symbols += x.size();
    }
    
    Trie trie = Trie(alphabetSize, positive, negative);
    DoubleTrie dt = DoubleTrie(alphabetSize, positive, negative);

    size_t cnt_nodes_trie = trie.nodes.size();
    size_t cnt_nodes_double_trie = dt.prefix_nodes.size() + dt.suffix_nodes.size();
    size_t cnt_links = dt.cnt_links();
    long long cnt_distinguishable_suffixes = dt.cnt_distinguishing_suffixes();
    double avg_overlap = trie.avg_overlap();
    double avg_word_len = 1.0 * cnt_symbols / (n + m);

    std::cout
        << alphabetSize << ","
        << n << ","
        << m << ","
        << avg_word_len << ","
        << cnt_symbols << ","
        << cnt_nodes_trie << ","
        << cnt_nodes_double_trie << ","
        << cnt_links << ","
        << cnt_distinguishable_suffixes << ","
        << avg_overlap
        << "\n";
}