#include "types.h"
#include <string>
#include <vector>

struct PrefixTrieNode {
    int children[alphabetSize];
    int parent;
    char char_from_par;

    std::vector<int> positive_suffixes;
    std::vector<int> negative_suffixes;
    

    PrefixTrieNode();
};

struct SuffixTrieNode {
    int children[alphabetSize];

    SuffixTrieNode();
};

class DoubleTrie {
    public:
        std::vector<PrefixTrieNode> prefix_nodes;
        std::vector<SuffixTrieNode> suffix_nodes;

        DoubleTrie();

        void insert(const std::string &s, Label label);

        std::vector<int> insert_to_pref_trie(const std::string &s);

        std::vector<int> insert_to_suf_trie(const std::string &s);

        int lookup(const std::string &s);
};
