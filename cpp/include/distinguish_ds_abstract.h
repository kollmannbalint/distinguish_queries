#pragma once
#include <string>
#include <vector>

class DistinguishDsAbstract {
    public:
        virtual void build(const std::vector<std::string> words) = 0;
        virtual std::string  query(const std::string &s1, const std::string &s2) const = 0;
};