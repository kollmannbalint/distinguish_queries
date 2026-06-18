#include "distinguish_ds_sqrt.h"
#include <algorithm>
#include <cmath>
#include <random>

std::vector<int> link_hist;

inline void fill_hist(const std::vector<int> &v, int val){
    for(int x : v)
        link_hist[x] = val;
}

inline int find_common_element_with_hist(const std::vector<int> &v){
    for(int x:v){
        if(link_hist[x] != 0)
            return x;
    }

    return -1;
}

void DistinguishDsSqrt::build(int alphabetSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words) {
    trie = DoubleTrie(alphabetSize, positive_words, negative_words);
    link_hist = std::vector<int>(trie.suffix_nodes.size(), 0);
    //threshold = (int)std::sqrt(trie.prefix_nodes.size() + trie.suffix_nodes.size());
    
    int tot_sym = 0;
    for(const std::string &w : positive_words)
        tot_sym += w.size();
    for(const std::string &w : negative_words)
        tot_sym += w.size();

    threshold = (int)std::sqrt(tot_sym);

    //in intersecting_pairs, the first key is positive, the second key is negative
    intersecting_pairs = std::unordered_map<long long, int>();

    for(int i = 0; i < trie.prefix_nodes.size(); i++){

        const DoubleTrieNode &node = trie.prefix_nodes[i];
        
        if(node.positive_links.size() >= threshold){
            fill_hist(node.positive_links, 1);

            for(int j = 0; j < trie.prefix_nodes.size(); j++){
                const DoubleTrieNode &node2 = trie.prefix_nodes[j];
                if(i == j || node2.negative_links.size() < threshold)
                    continue;
                
                int common_element = find_common_element_with_hist(node2.negative_links);
                if(common_element != -1)
                    intersecting_pairs.insert({ii_to_ll(i, j), common_element});
            }

            fill_hist(node.positive_links, 0);
        }

        if(node.negative_links.size() >= threshold){
            fill_hist(node.negative_links, 1);

            for(int j = 0; j < trie.prefix_nodes.size(); j++){
                const DoubleTrieNode &node2 = trie.prefix_nodes[j];
                if(i == j || node2.positive_links.size() < threshold)
                    continue;
                
                int common_element = find_common_element_with_hist(node2.positive_links);
                if(common_element != -1)
                    intersecting_pairs.insert({ii_to_ll(j, i), common_element});
            }

            fill_hist(node.negative_links, 0);
        }
    }
}

int DistinguishDsSqrt::intersect(const int n1, const int n2) const {

    const DoubleTrieNode &node1 = trie.prefix_nodes[n1];
    const DoubleTrieNode &node2 = trie.prefix_nodes[n2];

    bool is_n1_pos_big = node1.positive_links.size() >= threshold;
    bool is_n1_neg_big = node1.negative_links.size() >= threshold;
    bool is_n2_pos_big = node2.positive_links.size() >= threshold;
    bool is_n2_neg_big = node2.negative_links.size() >= threshold;

    //the positive set is the first key, the negative set is the second key for intersecting_pairs

    //check large sets
    if(is_n1_pos_big && is_n2_neg_big){
        auto it = intersecting_pairs.find(ii_to_ll(n1, n2));
        if(it != intersecting_pairs.end())
            return it->second;
    }

    if(is_n1_neg_big && is_n2_pos_big){
        auto it = intersecting_pairs.find(ii_to_ll(n2, n1));
        if(it != intersecting_pairs.end())
            return it->second;
    }

    //check small sets
    if(!is_n1_pos_big){
        for(int x : node1.positive_links){
            if(node2.negative_link_set.find(x) != node2.negative_link_set.end())
                return x;
        }
    }

    if(!is_n1_neg_big){
        for(int x : node1.negative_links){
            if(node2.positive_link_set.find(x) != node2.positive_link_set.end())
                return x;
        }
    }

    if(!is_n2_pos_big && is_n1_neg_big){
        for(int x : node2.positive_links){
            if(node1.negative_link_set.find(x) != node1.negative_link_set.end())
                return x;
        }
    }

    if(!is_n2_neg_big && is_n1_pos_big){
        for(int x : node2.negative_links){
            if(node1.positive_link_set.find(x) != node1.positive_link_set.end())
                return x;
        }
    }

    return -1;
}

std::string DistinguishDsSqrt::query(const std::string &s1, const std::string &s2) const {
    int n1 = trie.lookup(trie.prefix_nodes, s1);
    int n2 = trie.lookup(trie.prefix_nodes, s2);

    if(n1 == EMPTY_NODE || n2 == EMPTY_NODE)
        return "#";
    
    int suffix_idx = intersect(n1, n2);

    if(suffix_idx == -1)
        return "#";
    
    return trie.get_word(trie.suffix_nodes, suffix_idx);
}

size_t DistinguishDsSqrt::memory_usage() const {
    size_t total = 0;

    total += sizeof(int);
    total += trie.memory_usage();
    total += intersecting_pairs.bucket_count() * sizeof(void*);
    total += intersecting_pairs.size() * sizeof(std::pair<const long long, int>);

    return total;
}
