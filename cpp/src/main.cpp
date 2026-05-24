#include <iostream>
#include <vector>
#include <string>

#include "../include/distinguish_ds_abstract.h"
#include "../include/distinguish_ds_naiv.h"
#include "../include/distinguish_ds_precomp.h"

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> positive(n);

    for(int i = 0; i < n; i++) {
        std::cin>>positive[i];
    }

    int m;
    std::cin>>m;
    std::vector<std::string> negative(m);

    for(int i = 0; i < m; i++) {
        std::cin>>negative[i];
    }

    int q;
    std::cin >> q;
    std::vector<std::pair<std::string, std::string>> queries(q);

    for(int i = 0; i < q; i++) {
        std::cin >> queries[i].first >> queries[i].second;
    }

    DistinguishDsPrecomp naiv_ds = DistinguishDsPrecomp();

    std::vector<std::string> answers(q);

    //start timer

    naiv_ds.build(positive, negative);

    for(int i = 0; i < q; i++)
        answers[i] = naiv_ds.query(queries[i].first, queries[i].second);
    

    //end timer


    for(int i = 0; i < q; i++) {
        std::cout << (answers[i] != "" ? answers[i] : "$") << '\n';
    }
}