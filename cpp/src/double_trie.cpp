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
            std::reverse(prefix_path.begin(), prefix_path.end());

            for(int i = 0; i < prefix_path.size(); i++) {
                int pref_idx = prefix_path[i];
                int suf_idx = suffix_path[i];
                if(label == POSITIVE)
                    prefix_nodes[pref_idx].positive_suffixes.push_back(suf_idx);
                else if(label == NEGATIVE)
                    prefix_nodes[pref_idx].negative_suffixes.push_back(suf_idx);
            }
        }

        std::vector<int> insert_to_pref_trie(const std::string &s){
            std::vector<int> path;
            path.reserve(s.size() + 1);
            int cur = 0;
            path.push_back(cur);

            for(char ch : s) {
                int idx = ch - 'a';

                if (prefix_nodes[cur].children[idx] == EMPTY_NODE) {
                    prefix_nodes[cur].children[idx] = (int) prefix_nodes.size();
                    prefix_nodes.emplace_back();
                }

                cur = prefix_nodes[cur].children[idx];
                path.push_back(cur);
            }
            
            return path;
        }

        std::vector<int> insert_to_suf_trie(const std::string &s){
            std::vector<int> path;
            path.reserve(s.size() + 1);
            int cur = 0;
            path.push_back(cur);

            for(int i = s.size() - 1; i >= 0; i--) {
                int idx = s[i] - 'a';

                if (suffix_nodes[cur].children[idx] == EMPTY_NODE) {
                    suffix_nodes[cur].children[idx] = (int) suffix_nodes.size();
                    suffix_nodes.emplace_back();
                }

                cur = suffix_nodes[cur].children[idx];
                path.push_back(cur);
            }

            return path;
        }

        int lookup(const std::string &s){
            int cur = 0;
            for (char ch : s) {
                int idx = ch - 'a';

                cur = prefix_nodes[cur].children[idx];
                if (cur == EMPTY_NODE)
                    return EMPTY_NODE;
            }

            return cur;
        }
};
