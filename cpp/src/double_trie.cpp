#include "types.h"
#include <string>
#include <vector>

struct PrefixTrieNode {
    int children[alphabetSize];
    int parent;
    char char_from_par;

    std::vector<int> positive_suffixes;
    std::vector<int> negative_suffixes;
    

    PrefixTrieNode() {
        parent = EMPTY_NODE;
        char_from_par = '$';
        std::fill(std::begin(children), std::end(children), EMPTY_NODE);
    }
};

struct SuffixTrieNode {
    int children[alphabetSize];

    SuffixTrieNode() {
        std::fill(std::begin(children), std::end(children), EMPTY_NODE);
    }
};

class DoubleTrie {
    public:
        std::vector<PrefixTrieNode> prefix_nodes;
        std::vector<SuffixTrieNode> suffix_nodes;

        DoubleTrie(){
            prefix_nodes.emplace_back();
            prefix_nodes.emplace_back();  
        }

        void insert(const std::string &s, Label label){
            std::vector<int> prefix_path = insert_to_pref_trie(s);
            std::vector<int> suffix_path = insert_to_suf_trie(s);
            //put the ids of the suffix nodes to the corresponding prefix nodes
        }

        std::vector<int> insert_to_pref_trie(const std::string &s){
            //insert to pref trie and get the path
        }

        std::vector<int> insert_to_suf_trie(const std::string &s){
            //insert to suf trie and get the path
        }

        int lookup(const std::string &s){
            //lookup string in prefix tree
        }
};
