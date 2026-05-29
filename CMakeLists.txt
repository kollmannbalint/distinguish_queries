cmake_minimum_required(VERSION 3.16)
project(DistinguishDS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(trie
    cpp/src/trie.cpp
)

target_include_directories(trie
    PUBLIC cpp/include
)

add_library(double_trie
    cpp/src/double_trie.cpp
)

target_include_directories(double_trie
    PUBLIC cpp/include
)



add_library(dist_naiv
    cpp/src/distinguish_ds_naiv.cpp
)

target_include_directories(dist_naiv
    PUBLIC cpp/include
)

target_link_libraries(dist_naiv
    PUBLIC trie
)

add_library(dist_precomp
    cpp/src/distinguish_ds_precomp.cpp
)

target_include_directories(dist_precomp
    PUBLIC cpp/include
)

target_link_libraries(dist_precomp
    PUBLIC double_trie
)

add_library(dist_sqrt
    cpp/src/distinguish_ds_sqrt.cpp
)

target_include_directories(dist_sqrt
    PUBLIC cpp/include
)

target_link_libraries(dist_sqrt
    PUBLIC double_trie
)



add_executable(gen_query
    testing/generators/gen_query_from_double_trie.cpp
)

target_include_directories(gen_query
    PRIVATE cpp/include
)

target_link_libraries(gen_query
    PRIVATE double_trie
)

add_executable(main_app
    cpp/src/main.cpp
)

target_include_directories(main_app
    PRIVATE cpp/include
)

target_link_libraries(main_app
    PRIVATE trie
    PRIVATE double_trie
    PRIVATE dist_naiv
    PRIVATE dist_precomp
    PRIVATE dist_sqrt
)