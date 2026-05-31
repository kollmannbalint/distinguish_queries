#include "distinguish_ds_abstract.h"
#include "trie.h"


class DistinguishDsNaiv : public DistinguishDsAbstract {
    public:
        Trie trie;
        void build(const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words) override;
        std::string query(const std::string& s1, const std::string& s2) const override;
        size_t memory_usage() const override;
};