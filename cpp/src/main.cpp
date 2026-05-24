#include <iostream>
#include <vector>
#include <string>

#include <chrono>

#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "Psapi.lib")

#include "../include/distinguish_ds_abstract.h"
#include "../include/distinguish_ds_naiv.h"
#include "../include/distinguish_ds_precomp.h"
#include "../include/distinguish_ds_sqrt.h"

static size_t getPeakRSS() {
    PROCESS_MEMORY_COUNTERS info;

    GetProcessMemoryInfo(
        GetCurrentProcess(),
        &info,
        sizeof(info)
    );

    return info.PeakWorkingSetSize;
}

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

    DistinguishDsSqrt ds = DistinguishDsSqrt();
    

    std::vector<std::string> answers(q);

    using Clock = std::chrono::high_resolution_clock;
    auto build_start = Clock::now();

    ds.build(positive, negative);

    auto build_end = Clock::now();

    auto query_start = Clock::now();

    for(int i = 0; i < q; i++)
        answers[i] = ds.query(queries[i].first, queries[i].second);
    
    auto query_end = Clock::now();

     auto build_time =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(build_end - build_start).count();

    auto query_time =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(query_end - query_start).count();

    size_t peak_bytes = getPeakRSS();


    std::cout << build_time << '\n' << query_time << '\n' << peak_bytes << '\n';


    for(int i = 0; i < q; i++) {
        std::cout << (answers[i] != "" ? answers[i] : "$") << '\n';
    }
}