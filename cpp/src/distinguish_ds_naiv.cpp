#include "distinguish_ds_naiv.h"

void DistinguishDsNaiv::build(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words){
    trie = Trie(positive_words, negative_words);
}

std::string DistinguishDsNaiv::query(const std::string& s1, const std::string& s2) const {
    return trie.distinguish(s1, s2);
}