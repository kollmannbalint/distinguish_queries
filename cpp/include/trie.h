#include "types.h"
#include <string>
#include <vector>
#include <stack>
#include <tuple>
#include <algorithm>

struct TrieNode {
    std::vector<int> children;
    Label label;

    TrieNode(int alphaSize);
};

class Trie {
    public:
        std::vector<TrieNode> nodes;
        
        Trie();
        Trie(int alphaSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words);

        void insert(const std::string &s, Label label);
        int lookup(const std::string &s) const;
        std::string distinguish(const std::string &s1, const std::string &s2) const;
        int distinguish_dfs(const int node1, const int node2, std::string &distinguishing_suffix) const;
        size_t memory_usage() const;
    private:
        int alphabetSize;
};