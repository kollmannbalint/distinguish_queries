#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <chrono>

#include <windows.h>
#include <psapi.h>

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

#pragma comment(lib, "Psapi.lib")

#include "../include/distinguish_ds_abstract.h"
#include "../include/distinguish_ds_naiv.h"
#include "../include/distinguish_ds_precomp.h"
#include "../include/distinguish_ds_sqrt.h"
#include "distinguish_ds_db.h"

// static size_t getPeakRSS() {
//     PROCESS_MEMORY_COUNTERS info;

//     GetProcessMemoryInfo(
//         GetCurrentProcess(),
//         &info,
//         sizeof(info)
//     );

//     return info.PeakWorkingSetSize;
// }

static std::unordered_map<std::string, std::string>
load_db_config(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Cannot open config file: " + path);

    std::unordered_map<std::string, std::string> cfg;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        cfg[line.substr(0, pos)] = line.substr(pos + 1);
    }

    return cfg;
}

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    if(argc != 2){
        std::cerr << "Incorrect number of arguments" << '\n';
        return 1;
    }

    std::string impl = argv[1];

    std::unique_ptr<DistinguishDsAbstract> ds;

    if(impl == "naive")
        ds = std::make_unique<DistinguishDsNaiv>();
    else if(impl == "precomp")
        ds = std::make_unique<DistinguishDsPrecomp>();
    else if(impl == "sqrt")
        ds = std::make_unique<DistinguishDsSqrt>();
    else if(impl == "db"){
        std::string path = std::string(PROJECT_ROOT) + "/config/db_config.txt";
        auto config_map = load_db_config("config/db_config.txt");

        DBConfig cfg;

        cfg.conninfo =
            "host=" + config_map["host"] + " "
            "port=" + config_map["port"] + " "
            "dbname=" + config_map["dbname"] + " "
            "user=" + config_map["user"] + " "
            "password=" + config_map["password"];

        ds = std::make_unique<DistinguishDsDB>(cfg);
    }
    else{
        std::cerr << "Unknown implementation: " << impl << '\n';
        return 1;
    }

    int alphabetSize;
    std::cin >> alphabetSize;

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


    int q;
    std::cin >> q;
    std::vector<std::pair<std::string, std::string>> queries(q);

    for(int i = 0; i < q; i++) {
        std::cin >> queries[i].first >> queries[i].second;
        if(queries[i].first == "$")
            queries[i].first = "";
        if(queries[i].second == "$")
            queries[i].second = "";
    }
    

    std::vector<std::string> answers(q);

    using Clock = std::chrono::high_resolution_clock;
    auto build_start = Clock::now();
    ds->build(alphabetSize, positive, negative);
    auto build_end = Clock::now();

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


    size_t ds_memory = ds->memory_usage();

    size_t cnt_symbols = 0;
    for(const auto& x : positive){
        cnt_symbols += x.size();
    }
    for(const auto& x : negative){
        cnt_symbols += x.size();
    }

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