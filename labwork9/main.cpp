#include "MemoryPoolAllocator.h"

#include <chrono>
#include <iostream>
#include <list>
#include <map>

int main() {
    size_t n;
    std::cout << "How many objects you want to put in container?:";
    std::cin >> n;
    std::cout << "What allocator do you want to use(1 - MemoryPoolAllocator, 2 - std:allocator)?:";
    std::string str;
    std::cin >> str;
    if (str == "1") {
        int count;
        std::vector<Pool> pools;
        std::cout << "Enter number of pools:";
        std::cin >> count;
        for (int i = 0; i < count; ++i) {
            size_t block_number;
            size_t block_size;
            std::cout << "Enter number of blocks and their size(through a space):";
            std::cin >> block_number >> block_size;
            pools.emplace_back(Pool(block_number, block_size));
        }
        MemoryPoolAllocator<int> alloc(pools);
        MemoryPoolAllocator<int> list_alloc(pools);
        MemoryPoolAllocator<std::pair<const int, int>> map_alloc(pools);
        std::vector<int, MemoryPoolAllocator<int>> v(alloc);
        std::map<int, int, std::less<>, MemoryPoolAllocator<std::pair<const int, int>>> m(map_alloc);
        std::list<int, MemoryPoolAllocator<int>> l(list_alloc);
        auto start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            v.emplace(v.begin(), j);
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Program time on vector: " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        v.clear();
        start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            l.emplace(l.begin(), j);
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "Program time on list: " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        l.clear();
        start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            m[j] = j;
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "Program time: on map " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        m.clear();
    } else if (str == "2") {
        std::vector<int> a;
        std::map<int, int> m;
        std::list<int> l;
        auto start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            a.emplace(a.begin(), j);
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Program time on vector: " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        a.clear();
        start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            l.emplace(l.begin(), j);
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "Program time on list: " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        l.clear();
        start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < n; ++j) {
            m[j] = j;
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "Program time: on map " << std::chrono::duration<double, std::milli>(diff).count() << "ms"
                  << std::endl;
        m.clear();
    }
}


