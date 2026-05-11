#include "types.h"
#include "double_trie.h"
#include "distinguish_ds_sqrt.h"
#include <algorithm>

std::vector<int> link_hist;

void fill_hist(const std::vector<int> &v, int val){
    for(int x : v)
        link_hist[x] = val;
}

int find_common_element_with_hist(const std::vector<int> &v){
    for(int x:v){
        if(link_hist[x] != 0)
            return x;
    }

    return -1;
}

void DistinguishDsSqrt::build(const std::vector<std::pair<std::string, Label>>& words){
    trie = DoubleTrie(words);
    link_hist = std::vector<int>(trie.suffix_nodes.size(), 0);

    for(DoubleTrieNode &node : trie.prefix_nodes){
        std::sort(node.positive_links.begin(), node.positive_links.end());
        std::sort(node.negative_links.begin(), node.negative_links.end());
    }

    for(DoubleTrieNode &node : trie.suffix_nodes){
        std::sort(node.positive_links.begin(), node.positive_links.end());
        std::sort(node.negative_links.begin(), node.negative_links.end());
    }

    //in intersecting_pairs, the first key is positive, the second key is negative
    intersecting_pairs = std::unordered_map<long long, int>();

    for(int i = 0; trie.prefix_nodes.size(); i++){

        const DoubleTrieNode &node = trie.prefix_nodes[i];
        
        if(node.positive_links.size() >= threshold){
            fill_hist(node.positive_links, 1);

            for(int j = 0; trie.prefix_nodes.size(); j++){
                if(i == j)
                    continue;
                const DoubleTrieNode &node2 = trie.prefix_nodes[j];
                int common_element = find_common_element_with_hist(node2.negative_links);
                if(common_element != -1)
                    intersecting_pairs.insert({ii_to_ll(i, j), common_element});
            }

            fill_hist(node.positive_links, 0);
        }

        if(node.negative_links.size() >= threshold){
            fill_hist(node.negative_links, 1);

            for(int j = 0; trie.prefix_nodes.size(); j++){
                if(i == j)
                    continue;
                const DoubleTrieNode &node2 = trie.prefix_nodes[j];
                int common_element = find_common_element_with_hist(node2.positive_links);
                if(common_element != -1)
                    intersecting_pairs.insert({ii_to_ll(j, i), common_element});
            }

            fill_hist(node.negative_links, 0);
        }
    }
}

int intersect_small(const std::vector<int>& a, const std::vector<int>& b) {
    int i = 0, j = 0;

    while(i < a.size() && j < b.size()){
        if(a[i] == b[j])
            return a[i];

        if(a[i] < b[j])
            ++i;
        else
            ++j;
    }

    return -1;
}

int DistinguishDsSqrt::intersect(const int n1, const int n2) const {

    bool is_n1_pos_big = trie.prefix_nodes[n1].positive_links.size() >= threshold;
    bool is_n1_neg_big = trie.prefix_nodes[n1].negative_links.size() >= threshold;
    bool is_n2_pos_big = trie.prefix_nodes[n2].positive_links.size() >= threshold;
    bool is_n2_neg_big = trie.prefix_nodes[n2].negative_links.size() >= threshold;

    //the positive set is the first key, the negative set is the second key for intersecting_pairs
    if(is_n1_pos_big || is_n2_neg_big){
        auto it = intersecting_pairs.find(ii_to_ll(n1, n2));
        if(it != intersecting_pairs.end())
            return it->second;
    }

    if(is_n1_neg_big || is_n2_pos_big){
        auto it = intersecting_pairs.find(ii_to_ll(n2, n1));
        if(it != intersecting_pairs.end())
            return it->second;
    }

    if(!is_n1_pos_big && !is_n2_neg_big){
        int res = intersect_small(trie.prefix_nodes[n1].positive_links, trie.prefix_nodes[n2].negative_links);
        if(res != -1)
            return res;
    }

    if(!is_n1_neg_big && !is_n2_pos_big){
        int res = intersect_small(trie.prefix_nodes[n2].positive_links, trie.prefix_nodes[n1].negative_links);
        if(res != -1)
            return res;
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