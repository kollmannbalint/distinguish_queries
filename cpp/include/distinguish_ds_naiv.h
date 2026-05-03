#include "trie.h"
#include "types.h"
#include "distinguish_ds_abstract.h"


class DistingusihDsNaiv : public DistinguishDsAbstract {
    public:
        Trie trie;
        void build(const std::vector<std::pair<std::string, Label>>& words) override;
        std::string query(const std::string& s1, const std::string& s2) const override;
};