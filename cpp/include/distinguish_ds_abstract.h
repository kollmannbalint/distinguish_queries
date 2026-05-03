#pragma once
#include "types.h"
#include <string>
#include <vector>

class DistinguishDsAbstract {
    public:
        virtual void build(const std::vector<std::pair<std::string, Label>> &words) = 0;
        virtual std::string  query(const std::string &s1, const std::string &s2) const = 0;
};