#include "types.h"
#include <string>
#include <vector>
#include <stack>
#include <tuple>
#include <algorithm>

struct TrieNode {
    int children[alphabetSize];
    Label label;

    TrieNode();
};

class Trie {
    public:
        std::vector<TrieNode> nodes;
        
        Trie();
        Trie(const std::vector<std::pair<std::string, Label>> &words);

        void insert(const std::string &s, Label label);

        int lookup(const std::string &s) const;

        std::string distinguish(const std::string &s1, const std::string &s2) const;
};