#include <stdexcept>
#include <iostream>
#include <unordered_map>

#include "distinguish_ds_db.h"


DistinguishDsDB::DistinguishDsDB(const DBConfig& cfg)
    : conn(nullptr),
      config(cfg)
{
    conn = PQconnectdb(config.conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK)
    {
        std::string msg = PQerrorMessage(conn);

        PQfinish(conn);
        conn = nullptr;

        throw std::runtime_error(
            "PostgreSQL connection failed: " + msg
        );
    }
}

DistinguishDsDB::~DistinguishDsDB()
{
    if (conn)
    {
        PQfinish(conn);
        conn = nullptr;
    }
}

void DistinguishDsDB::build(int alphabetSize, const std::vector<std::string> &positive_words, const std::vector<std::string> &negative_words)
{
    trie = DoubleTrie(alphabetSize, positive_words, negative_words);

    {
        PGresult* res = PQexec(conn, "TRUNCATE TABLE links;");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            std::string err = PQerrorMessage(conn);
            PQclear(res);
            throw std::runtime_error("TRUNCATE failed: " + err);
        }
        PQclear(res);
    }


    {
        PGresult* res = PQexec(conn,
            "COPY links (prefix_node_id, suffix_node_id, polarity) FROM STDIN");

        if (PQresultStatus(res) != PGRES_COPY_IN)
        {
            std::string err = PQerrorMessage(conn);
            PQclear(res);
            throw std::runtime_error("COPY start failed: " + err);
        }
        PQclear(res);
    }

    std::string buffer;
    buffer.reserve(4 * 1024 * 1024);

    auto flush = [&]()
    {
        if (buffer.empty()) return;

        if (PQputCopyData(conn, buffer.c_str(), buffer.size()) != 1)
            throw std::runtime_error("PQputCopyData failed");

        buffer.clear();
    };

    for (int i = 0; i < (int)trie.prefix_nodes.size(); i++)
    {
        const DoubleTrieNode &node = trie.prefix_nodes[i];

        for (int x : node.positive_links)
        {
            buffer += std::to_string(i);
            buffer += '\t';
            buffer += std::to_string(x);
            buffer += '\t';
            buffer += "1\n";

            if (buffer.size() >= (4 * 1024 * 1024))
                flush();
        }

        for (int x : node.negative_links)
        {
            buffer += std::to_string(i);
            buffer += '\t';
            buffer += std::to_string(x);
            buffer += '\t';
            buffer += "-1\n";

            if (buffer.size() >= (4 * 1024 * 1024))
                flush();
        }
    }

    flush();

    if (PQputCopyEnd(conn, nullptr) != 1)
        throw std::runtime_error("PQputCopyEnd failed");

    PGresult* res = PQgetResult(conn);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string err = PQerrorMessage(conn);
        PQclear(res);
        throw std::runtime_error("COPY finish failed: " + err);
    }

    PQclear(res);
}

std::string DistinguishDsDB::query(const std::string& s1, const std::string& s2) const {
    int n1 = trie.lookup(trie.prefix_nodes, s1);
    int n2 = trie.lookup(trie.prefix_nodes, s2);

    if (n1 == -1 || n2 == -1)
        return "#";

    std::string p1 = std::to_string(n1);
    std::string p2 = std::to_string(n2);

    const char* params[2];
    params[0] = p1.c_str();
    params[1] = p2.c_str();

    PGresult* res = PQexecParams(
        conn,
        R"sql(
            SELECT l1.suffix_node_id
            FROM links l1
            JOIN links l2
              ON l1.suffix_node_id = l2.suffix_node_id
            WHERE l1.prefix_node_id = $1
              AND l2.prefix_node_id = $2
              AND (
                   (l1.polarity =  1 AND l2.polarity = -1)
                OR (l1.polarity = -1 AND l2.polarity =  1)
              )
            LIMIT 1
        )sql",
        2,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::string err = PQerrorMessage(conn);
        PQclear(res);
        throw std::runtime_error("Query failed: " + err);
    }

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return "#";
    }

    int suffix_id = std::stoi(PQgetvalue(res, 0, 0));

    PQclear(res);

    return trie.get_word(trie.suffix_nodes, suffix_id);
}


size_t DistinguishDsDB::memory_usage() const {
    return trie.memory_usage();
}

size_t DistinguishDsDB::cnt_nodes() const {
    return trie.prefix_nodes.size() + trie.suffix_nodes.size();
}

size_t DistinguishDsDB::cnt_links() const {
    size_t res = 0;
    for(const auto& x : trie.prefix_nodes){
        res += x.positive_links.size() + x.negative_links.size();
    }
    return res;
}

long long DistinguishDsDB::cnt_distinguishable_suffixes() const {
    long long res = 0;
    for(const auto& x: trie.prefix_nodes){
        res += 1ll * x.positive_links.size() * x.negative_links.size();
    }
    return res;
}