#include "distinguish_ds_precomp.h"
#include <iostream>

void DistinguishDsPrecomp::build(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words){
    trie = DoubleTrie(positive_words, negative_words);
    intersecting_pairs = std::unordered_map<long long, int>();

    int idx = 0;
    for (DoubleTrieNode &node : trie.suffix_nodes){
        for (int p : node.positive_links)
            for (int n : node.negative_links){
                intersecting_pairs.insert({ii_to_ll(p,n),idx});
            }
                
        ++idx;
    }
}


std::string DistinguishDsPrecomp::query(const std::string &s1, const std::string &s2) const {
    int n1 = trie.lookup(trie.prefix_nodes, s1);
    int n2 = trie.lookup(trie.prefix_nodes, s2);

    if(n1 == EMPTY_NODE || n2 == EMPTY_NODE)
        return "#";
    
    auto it = intersecting_pairs.find(ii_to_ll(n1,n2));

    if(it != intersecting_pairs.end())
        return trie.get_word(trie.suffix_nodes, it->second);

    it = intersecting_pairs.find(ii_to_ll(n2,n1));

    if(it != intersecting_pairs.end())
        return trie.get_word(trie.suffix_nodes, it->second);

    return "#";
}

size_t DistinguishDsPrecomp::memory_usage() const {
    size_t total = 0;

    total += trie.memory_usage();
    total += intersecting_pairs.bucket_count() * sizeof(void*);
    total += intersecting_pairs.size() * sizeof(std::pair<const long long, int>);

    return total;
}