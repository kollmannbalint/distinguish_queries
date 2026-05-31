#include "trie.h"
#include <iostream>

TrieNode::TrieNode() : label(NONE){
    std::fill(std::begin(children), std::end(children), EMPTY_NODE);
}


Trie::Trie(){
    nodes.emplace_back();
}

Trie::Trie(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words){
    nodes.emplace_back();
    for(const std::string& s : positive_words) {
        insert(s, POSITIVE);
    }

    for(const std::string& s : negative_words) {
        insert(s, NEGATIVE);
    }
}

void Trie::insert(const std::string &s, Label label){
    int cur = 0;
    for(char ch : s) {
        int idx = ch - 'a';

        if(nodes[cur].children[idx] == EMPTY_NODE) {
            nodes[cur].children[idx] = (int) nodes.size();
            nodes.emplace_back();
        }

        cur = nodes[cur].children[idx];
    }
    
    nodes[cur].label = label;
}

int Trie::lookup(const std::string &s) const {
    int cur = 0;
    for(char ch : s) {
        int idx = ch - 'a';

        cur = nodes[cur].children[idx];
        if (cur == EMPTY_NODE)
            return EMPTY_NODE;
    }

    return cur;
}

int Trie::distinguish_dfs(const int node1, const int node2, std::string &distinguishing_suffix) const {
    if(isContradiction(nodes[node1].label, nodes[node2].label)){
        distinguishing_suffix = "";
        return 0;
    }

    for(int i=0;i<alphabetSize;i++){
            int next_node1 = nodes[node1].children[i];
            int next_node2 = nodes[node2].children[i];
            if(next_node1 != EMPTY_NODE && next_node2 != EMPTY_NODE){
                int res = distinguish_dfs(next_node1, next_node2, distinguishing_suffix);
                if(res != -1){
                    distinguishing_suffix.push_back('a' + i);
                    return 0;
                }
            }
        }
    
    return -1;
}

std::string Trie::distinguish(const std::string &s1, const std::string &s2) const {
    int pref_node1 = lookup(s1); 
    int pref_node2 = lookup(s2);

    if(pref_node1 == EMPTY_NODE || pref_node2 == EMPTY_NODE)
        return "#";

    std::string distinguishing_suffix = "";

    int res = distinguish_dfs(pref_node1, pref_node2, distinguishing_suffix);

    if (res == -1)
        return "#";
    
    std::reverse(distinguishing_suffix.begin(), distinguishing_suffix.end());
    
    return distinguishing_suffix;
}

size_t Trie::memory_usage() const {
    return nodes.capacity() * sizeof(TrieNode);
}