#include "trie.h"
#include "distinguish_ds_naiv.h"

void DistingusihDsNaiv::build(const std::vector<std::pair<std::string, Label>>& words){
    trie = Trie(words);
}

std::string DistingusihDsNaiv::query(const std::string& s1, const std::string& s2) const {
    return trie.distinguish(s1, s2);
}