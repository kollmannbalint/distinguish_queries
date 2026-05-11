#include "double_trie.h"
#include "types.h"
#include "distinguish_ds_abstract.h"
#include <unordered_map>

class DistinguishDsSqrt : public DistinguishDsAbstract {
    public:
        DoubleTrie trie;
        std::unordered_map<long long, int> intersecting_pairs;
        int threshold;

        void build(const std::vector<std::pair<std::string, Label>> &words) override;
        std::string query(const std::string &s1, const std::string &s2) const override;
        int intersect(const int n1, const int n2) const;
};