#pragma once

enum Label {
    NONE = 0,
    POSITIVE = 1,
    NEGATIVE = -1
};

inline bool isContradiction(Label a, Label b){
    return a * b == -1;
}