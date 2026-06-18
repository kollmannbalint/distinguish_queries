#pragma once

#include <libpq-fe.h>
#include <vector>
#include <string>
#include "double_trie.h"
#include "distinguish_ds_abstract.h"
#include "types.h"

struct DBConfig {
    std::string conninfo;
};

class DistinguishDsDB : public DistinguishDsAbstract {
public:
    DoubleTrie trie;
    mutable std::vector<std::pair<int,int>> queries;

    DistinguishDsDB(const DBConfig& cfg);
    ~DistinguishDsDB();

    void build(int alphabetSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words) override;
    std::string query(const std::string& s1, const std::string& s2) const override;
    size_t memory_usage() const override;
    std::vector<std::string> execute_queries();

private:
    PGconn* conn;
    DBConfig config;
};