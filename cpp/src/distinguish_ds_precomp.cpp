#include "types.h"
#include "double_trie.h"
#include "distinguish_ds_precomp.h"

void DistinguishDsPrecomp::build(const std::vector<std::pair<std::string, Label>>& words){
    trie = DoubleTrie(words);
    intersecting_pairs = std::unordered_map<long long, int>();

    int idx = 0;
    for (DoubleTrieNode &node : trie.suffix_nodes){
        for (int p : node.positive_links)
            for (int n : node.negative_links)
                intersecting_pairs.insert({ii_to_ll(p,n),idx});
        ++idx;
    }
}


std::string DistinguishDsPrecomp::query(const std::string &s1, const std::string &s2) const {
    int n1 = trie.lookup(trie.prefix_nodes, s1);
    int n2 = trie.lookup(trie.prefix_nodes, s2);

    if(n1 == EMPTY_NODE || n2 == EMPTY_NODE)
        return "#";
    
    auto it = intersecting_pairs.find(ii_to_ll(n1,n2));

    if(it == intersecting_pairs.end())
        return "#";
    
    return trie.get_word(trie.suffix_nodes, it->second);
}