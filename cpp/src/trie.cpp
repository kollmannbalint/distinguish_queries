#include "types.h"
#include <vector>
#include <string>
#include <stack>

const int K = 26;

struct TrieNode {
    TrieNode* child[K];

    Label label;

    TrieNode() {
        label = NONE;
        memset(child, 0, sizeof(child));
    }
};

class Trie{
    public:
        TrieNode* root;

        Trie(){
            root = new TrieNode();
        }

        void insert(const std::string &s, Label label){
            TrieNode* cur = root;
            for(char ch : s){
                int idx = ch - 'a';

                if(!cur->child[idx])
                    cur->child[idx] = new TrieNode();

                cur = cur->child[idx];
            }

            cur->label = label;
        }

        TrieNode* lookup(const std::string &s){
            TrieNode* cur = root;
            for(char ch : s){
                int idx = ch - 'a';
                if(!cur->child[idx]) 
                    return nullptr;
                cur = cur->child[idx];
            }
            return cur;
        }

        std::string distinguish(const std::string &w1, const std::string &w2){
            TrieNode* pref_node1, * pref_node2;

            if(!(pref_node1 = lookup(w1)) || !(pref_node2 = lookup(w2)))
                return nullptr;

            std::string distinguishing_suffix = "";
            std::stack<TrieNode*> dfs_stack1;
            dfs_stack1.push(pref_node1);
            std::stack<TrieNode*> dfs_stack2;
            dfs_stack2.push(pref_node2);
            std::string char_stack = "$";

            while(!dfs_stack1.empty()){
                TrieNode* cur_node1 = dfs_stack1.top();
                dfs_stack1.pop();
                TrieNode* cur_node2 = dfs_stack2.top();
                dfs_stack2.pop();
                distinguishing_suffix.push_back(char_stack.back());
                char_stack.pop_back();

                if(isContradiction(cur_node1->label, cur_node2->label))
                    return distinguishing_suffix;

                for(int i=0;i<K;i++){
                    if(cur_node1->child[i] != nullptr && cur_node2->child[i] != nullptr){
                        dfs_stack1.push(cur_node1->child[i]);
                        dfs_stack2.push(cur_node2->child[i]);
                        char_stack.push_back('a' + i);
                    }
                }

                distinguishing_suffix.pop_back();
            }

            return nullptr;
        }
};