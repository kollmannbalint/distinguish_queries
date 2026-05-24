#include "double_trie.h"
#include "types.h"
#include "distinguish_ds_abstract.h"
#include <unordered_map>

class DistinguishDsPrecomp : public DistinguishDsAbstract {
    public:
        DoubleTrie trie;
        std::unordered_map<long long, int> intersecting_pairs;
        void build(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words) override;
        std::string query(const std::string &s1, const std::string &s2) const override;
};