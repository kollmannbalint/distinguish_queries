#include "trie.h"


TrieNode::TrieNode() : label(NONE){
    std::fill(std::begin(children), std::end(children), EMPTY_NODE);
}


Trie::Trie(){
    nodes.emplace_back();
}

Trie::Trie(const std::vector<std::pair<std::string, Label>> &words){
    nodes.emplace_back();
    for (const auto& [w, label] : words) {
        insert(w, label);
    }
}

void Trie::insert(const std::string &s, Label label){
    int cur = 0;
    for(char ch : s) {
        int idx = ch - 'a';

        if (nodes[cur].children[idx] == EMPTY_NODE) {
            nodes[cur].children[idx] = (int) nodes.size();
            nodes.emplace_back();
        }

        cur = nodes[cur].children[idx];
    }
    
    nodes[cur].label = label;
}

int Trie::lookup(const std::string &s) const {
    int cur = 0;
    for (char ch : s) {
        int idx = ch - 'a';

        cur = nodes[cur].children[idx];
        if (cur == EMPTY_NODE)
            return EMPTY_NODE;
    }

    return cur;
}

std::string Trie::distinguish(const std::string &s1, const std::string &s2) const {
    int pref_node1 = lookup(s1); 
    int pref_node2 = lookup(s2);

    if(pref_node1 == EMPTY_NODE || pref_node2 == EMPTY_NODE)
        return "#";

    std::string distinguishing_suffix = "";
    std::stack<std::tuple<int, int, char>> dfs_stack;
    dfs_stack.push({pref_node1, pref_node2, '$'});

    

    while(!dfs_stack.empty()){
        auto [cur_node1, cur_node2, cur_char] = dfs_stack.top();
        dfs_stack.pop();
        distinguishing_suffix.push_back(cur_char);

        if(isContradiction(nodes[cur_node1].label, nodes[cur_node2].label))
            return distinguishing_suffix;

        for(int i=0;i<alphabetSize;i++){
            if(nodes[cur_node1].children[i] != EMPTY_NODE && nodes[cur_node2].children[i] != EMPTY_NODE){
                dfs_stack.push({nodes[cur_node1].children[i], nodes[cur_node2].children[i], 'a' + i});
            }
        }

        distinguishing_suffix.pop_back();
    }

    return "#";
}