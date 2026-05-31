#include "double_trie.h"
#include <algorithm>
#include <iostream>

DoubleTrieNode::DoubleTrieNode() {
    parent = EMPTY_NODE;
    char_from_par = '$';
    std::fill(std::begin(children), std::end(children), EMPTY_NODE);
}

size_t DoubleTrieNode::memory_usage_dynamic() const {
    size_t total = 0;

    total += positive_links.capacity() * sizeof(int);
    total += negative_links.capacity() * sizeof(int);
    total += positive_link_set.bucket_count() * sizeof(void*);
    total += negative_link_set.bucket_count() * sizeof(void*);
    total += positive_link_set.size() * sizeof(int);
    total += negative_link_set.size() * sizeof(int);


    return total;
}

DoubleTrie::DoubleTrie(){
    prefix_nodes.emplace_back();
    suffix_nodes.emplace_back();  
}

DoubleTrie::DoubleTrie(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words){
    prefix_nodes.emplace_back();
    suffix_nodes.emplace_back();

    for(const std::string s : positive_words){
        insert(s, POSITIVE);
    }

     for (const std::string s : negative_words) {
        insert(s, NEGATIVE);
    }
}

void printvec(std::vector<int> v){
    for(int x : v) std::cout << x << ' ';
}

void DoubleTrie::insert(const std::string &s, Label label){
    std::vector<int> prefix_path = insert_to_trie(prefix_nodes, s);
    std::string t = s;
    std::reverse(t.begin(), t.end());
    std::vector<int> suffix_path = insert_to_trie(suffix_nodes, t);
    std::reverse(prefix_path.begin(), prefix_path.end());

    for(int i = 0; i < prefix_path.size(); i++) {
        int pref_idx = prefix_path[i];
        int suf_idx = suffix_path[i];
        if(label == POSITIVE){
            prefix_nodes[pref_idx].positive_links.push_back(suf_idx);
            suffix_nodes[suf_idx].positive_links.push_back(pref_idx);
            prefix_nodes[pref_idx].positive_link_set.insert(suf_idx);
            //the link sets of suffix nodes are not used anywhere, so they are not filled
            //suffix_nodes[suf_idx].positive_link_set.insert(pref_idx);
        }
        else if(label == NEGATIVE){
            prefix_nodes[pref_idx].negative_links.push_back(suf_idx);
            suffix_nodes[suf_idx].negative_links.push_back(pref_idx);
            prefix_nodes[pref_idx].negative_link_set.insert(suf_idx);
            //the link sets of suffix nodes are not used anywhere, so they are not filled
        }
    }
}

std::vector<int> DoubleTrie::insert_to_trie(std::vector<DoubleTrieNode> &trie, const std::string &s){
    std::vector<int> path;
    path.reserve(s.size() + 1);
    int cur = 0;
    path.push_back(cur);

    for(char ch : s) {
        int idx = ch - 'a';

        if (trie[cur].children[idx] == EMPTY_NODE) {
            trie[cur].children[idx] = (int) trie.size();
            trie.emplace_back();
            int nxt = trie[cur].children[idx];
            trie[nxt].parent = cur;
            trie[nxt].char_from_par = ch;
        }

        cur = trie[cur].children[idx];
        path.push_back(cur);
    }
    
    return path;
}

int DoubleTrie::lookup(const std::vector<DoubleTrieNode> &trie, const std::string &s) const {
    int cur = 0;
    for (char ch : s) {
        int idx = ch - 'a';

        cur = trie[cur].children[idx];
        if (cur == EMPTY_NODE)
            return EMPTY_NODE;
    }

    return cur;
}

std::string DoubleTrie::get_word(const std::vector<DoubleTrieNode> &trie, int id) const {
    std::string word = "";
    while(id != 0){
        word.push_back(trie[id].char_from_par);
        id = trie[id].parent;
    }
    return word;
}

size_t DoubleTrie::memory_usage() const {
    size_t total = 0;

    total += prefix_nodes.capacity() * sizeof(DoubleTrieNode);
    total += suffix_nodes.capacity() * sizeof(DoubleTrieNode);

    for (const auto& node : prefix_nodes)
        total += node.memory_usage_dynamic();

    for (const auto& node : suffix_nodes)
        total += node.memory_usage_dynamic();

    return total;
}