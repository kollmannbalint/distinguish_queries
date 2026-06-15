#include "double_trie.h"
#include "types.h"
#include "distinguish_ds_abstract.h"
#include <unordered_map>

class DistinguishDsSqrt : public DistinguishDsAbstract {
    public:
        DoubleTrie trie;
        std::unordered_map<long long, int> intersecting_pairs;
        int threshold;

        void build(int alphabetSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words) override;
        std::string query(const std::string &s1, const std::string &s2) const override;
        int intersect(const int n1, const int n2) const;
        size_t memory_usage() const override;
        size_t cnt_nodes() const override;
        size_t cnt_links() const override;
        long long cnt_distinguishable_suffixes() const override;
};