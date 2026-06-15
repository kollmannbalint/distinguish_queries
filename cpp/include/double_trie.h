#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_set>

struct DoubleTrieNode {
    std::vector<int> children;
    int parent;
    char char_from_par;

    std::vector<int> positive_links;
    std::vector<int> negative_links;
    std::unordered_set<int> positive_link_set;
    std::unordered_set<int> negative_link_set;

    size_t memory_usage_dynamic() const;

    DoubleTrieNode(int alphaSize);
};

class DoubleTrie {
    public:
        std::vector<DoubleTrieNode> prefix_nodes;
        std::vector<DoubleTrieNode> suffix_nodes;

        DoubleTrie();
        DoubleTrie(int alphaSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words);
        void insert(const std::string &s, Label label);
        std::vector<int> insert_to_trie(std::vector<DoubleTrieNode> &trie_nodes, const std::string &s);
        int lookup(const std::vector<DoubleTrieNode> &trie, const std::string &s) const;
        std::string get_word(const std::vector<DoubleTrieNode> &trie, int id) const;
        size_t memory_usage() const;
    private:
        int alphabetSize;
};
