#include "double_trie.h"


DoubleTrieNode::DoubleTrieNode() {
    parent = EMPTY_NODE;
    char_from_par = '$';
    std::fill(std::begin(children), std::end(children), EMPTY_NODE);
}

DoubleTrie::DoubleTrie(){
    prefix_nodes.emplace_back();
    prefix_nodes.emplace_back();  
}

DoubleTrie::DoubleTrie(const std::vector<std::pair<std::string, Label>> &words){
    prefix_nodes.emplace_back();
    prefix_nodes.emplace_back();
    for (const auto& [w, label] : words) {
        insert(w, label);
    }
}

void DoubleTrie::insert(const std::string &s, Label label){
    std::vector<int> prefix_path = insert_to_trie(prefix_nodes, s);
    std::vector<int> suffix_path = insert_to_trie(suffix_nodes, s);
    std::reverse(prefix_path.begin(), prefix_path.end());

    for(int i = 0; i < prefix_path.size(); i++) {
        int pref_idx = prefix_path[i];
        int suf_idx = suffix_path[i];
        if(label == POSITIVE){
            prefix_nodes[pref_idx].positive_links.push_back(suf_idx);
            suffix_nodes[suf_idx].positive_links.push_back(pref_idx);
        }
        else if(label == NEGATIVE){
            prefix_nodes[pref_idx].negative_links.push_back(suf_idx);
            suffix_nodes[suf_idx].negative_links.push_back(pref_idx);
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
