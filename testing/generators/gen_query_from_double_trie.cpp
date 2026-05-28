#include "../../cpp/include/double_trie.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

std::mt19937 gen(8324932);

int random_int(int low, int high) {
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

int main(){
    int n;
    std::cin >> n;
    std::vector<std::string> positive(n);

    for(int i = 0; i < n; i++) {
        std::cin>>positive[i];
    }

    int m;
    std::cin>>m;
    std::vector<std::string> negative(m);

    for(int i = 0; i < m; i++) {
        std::cin>>negative[i];
    }

    DoubleTrie dt = DoubleTrie(positive, negative);

    std::vector<int> distinguishable_suffix_nodes;
    
    for(int i = 0; i < dt.suffix_nodes.size();i++){
        const DoubleTrieNode &node = dt.suffix_nodes[i];
        if(!node.positive_links.empty() && !node.negative_links.empty());
    }

    int cnt_queries = 0;
    int tot_word_length = 0;

    int max_queries = 120;
    int max_tot_word_length = 2000;

    while(!distinguishable_suffix_nodes.empty() && cnt_queries < max_queries && tot_word_length < max_tot_word_length){
        int suf_id = random_int(0, distinguishable_suffix_nodes.size() - 1);
        DoubleTrieNode &node =  dt.suffix_nodes[ distinguishable_suffix_nodes[suf_id] ];
        int pref1_id = random_int(0, node.positive_links.size() - 1);
        int pref2_id = random_int(0, node.negative_links.size() - 1);

        int ord = random_int(0,1);
        if(ord) std::swap(pref1_id, pref2_id);

        std::string w1 = dt.get_word(dt.prefix_nodes, pref1_id);
        std::reverse(w1.begin(), w1.end());

        std::string w2 = dt.get_word(dt.prefix_nodes, pref2_id);
        std::reverse(w2.begin(), w2.end());

        std::cout << w1 << ' ' << w2 << '\n';

        ++cnt_queries;
        tot_word_length += w1.size() + w2.size();
    }
}