#pragma once

enum Label {
    NONE = 0,
    POSITIVE = 1,
    NEGATIVE = -1
};

inline bool isContradiction(Label a, Label b){
    return a * b == -1;
}

constexpr int alphabetSize = 26;
constexpr int EMPTY_NODE = -1;

inline long long ii_to_ll(long long a, long long b){
    return (a << 32) | b;
}