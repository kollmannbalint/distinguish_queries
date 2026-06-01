#include "../../cpp/include/double_trie.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

int max_queries = 120;
int max_tot_word_length = 2000;
int randseed = 8324932;

std::mt19937 gen;

int random_int(int low, int high) {
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> randseed >> max_queries >> max_tot_word_length;
    gen.seed(randseed);

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

    DoubleTrie dt = DoubleTrie(positive, negative);

    std::vector<int> distinguishable_suffix_nodes;
    
    for(int i = 0; i < dt.suffix_nodes.size(); i++){
        const DoubleTrieNode &node = dt.suffix_nodes[i];
        if(!node.positive_links.empty() && !node.negative_links.empty()){
            distinguishable_suffix_nodes.push_back(i);
        }
    }

    int cnt_queries = 0;
    int tot_word_length = 0;

    while(!distinguishable_suffix_nodes.empty() && cnt_queries < max_queries && tot_word_length < max_tot_word_length){
        int suf_id = distinguishable_suffix_nodes[random_int(0, distinguishable_suffix_nodes.size() - 1)];
        DoubleTrieNode &node =  dt.suffix_nodes[suf_id];
        int pref1_id = node.positive_links[random_int(0, node.positive_links.size() - 1)];
        int pref2_id = node.negative_links[random_int(0, node.negative_links.size() - 1)];

        ++cnt_queries;

        int ord = random_int(0,1);
        if(ord) std::swap(pref1_id, pref2_id);

        std::string w1 = dt.get_word(dt.prefix_nodes, pref1_id);
        std::reverse(w1.begin(), w1.end());

        std::string w2 = dt.get_word(dt.prefix_nodes, pref2_id);
        std::reverse(w2.begin(), w2.end());

        if(w1 == "")
            w1 = "$";
        if(w2 == "")
            w2 = "$";

        std::cout << w1 << ' ' << w2 << '\n';

        
        tot_word_length += w1.size() + w2.size();
    }
}