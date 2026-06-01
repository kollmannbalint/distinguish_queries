#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <chrono>

#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "Psapi.lib")

#include "../include/distinguish_ds_abstract.h"
#include "../include/distinguish_ds_naiv.h"
#include "../include/distinguish_ds_precomp.h"
#include "../include/distinguish_ds_sqrt.h"

// static size_t getPeakRSS() {
//     PROCESS_MEMORY_COUNTERS info;

//     GetProcessMemoryInfo(
//         GetCurrentProcess(),
//         &info,
//         sizeof(info)
//     );

//     return info.PeakWorkingSetSize;
// }

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    if(argc != 2){
        std::cerr << "Incorrect number of arguments" << '\n';
        return 1;
    }

    //std::cerr << "Start the program" << '\n';

    std::string impl = argv[1];

    std::unique_ptr<DistinguishDsAbstract> ds;

    if(impl == "naive")
        ds = std::make_unique<DistinguishDsNaiv>();
    else if(impl == "precomp")
        ds = std::make_unique<DistinguishDsPrecomp>();
    else if(impl == "sqrt")
        ds = std::make_unique<DistinguishDsSqrt>();
    else{
        std::cerr << "Unknown implementation: " << impl << '\n';
        return 1;
    }


    int n;
    std::cin >> n;
    std::vector<std::string> positive(n);

    for(int i = 0; i < n; i++) {
        std::cin>>positive[i];
        if(positive[i] == "$")
            positive[i] = "";
    }

    int m;
    std::cin>>m;
    std::vector<std::string> negative(m);

    for(int i = 0; i < m; i++) {
        std::cin>>negative[i];
        if(negative[i] == "$")
            negative[i] = "";
    }

    //std::cerr << "Read traces done" << '\n';

    int q;
    std::cin >> q;
    std::cout << q << ' ' << "val of q" << '\n';
    std::vector<std::pair<std::string, std::string>> queries(q);

    //std::cerr << "Create queries" << '\n';

    for(int i = 0; i < q; i++) {
        std::cin >> queries[i].first >> queries[i].second;
        if(queries[i].first == "$")
            queries[i].first = "";
        if(queries[i].second == "$")
            queries[i].second = "";
    }
    
    //std::cerr << "Read data done" << '\n';

    //std::vector<std::string> answers(q);

    using Clock = std::chrono::high_resolution_clock;
    auto build_start = Clock::now();
    ds->build(positive, negative);
    auto build_end = Clock::now();

    std::cerr << "Build data structure done" << '\n';

    auto query_start = Clock::now();
    size_t tot_length = 0;

    for(int i = 0; i < q; i++)
        tot_length += ds->query(queries[i].first, queries[i].second).size();
        //answers[i] = ds->query(queries[i].first, queries[i].second);
    
    auto query_end = Clock::now();

     auto build_time =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(build_end - build_start).count();

    auto query_time =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(query_end - query_start).count();

    std::cerr << "Answer queries done" << '\n';


    size_t ds_memory = ds->memory_usage();

    //Optionl todo: maybe also output total nodes, links and nodes in the distinguishing trie sets

    std::cout
        << impl << ","
        << n << ","
        << m << ","
        << q << ","
        << build_time << ","
        << query_time << ","
        << ds_memory << ","
        << tot_length
        << "\n";


    // for(int i = 0; i < q; i++) {
    //     std::cout << (answers[i] != "" ? answers[i] : "$") << '\n';
    // }
}