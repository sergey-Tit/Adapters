#include <iostream>
#include <lib\Ranges.cpp>
#include <vector>
#include <ranges>
#include <set>
#include <map>
#include <forward_list>

int main() {
    std::vector<int> v = {3, 2, 4};
    std::vector<int> res, ans = {21, 2};
    for (auto el : v | take_view(3) | transform_view([](int x) { return std::make_pair(x, x); }) | reverse_view()) {
        std::cout << el.first << ' ' << el.second << '\n';
    }
    /*
    auto sl1 = reverse(sl);
    auto sl2 = filter(sl1, [](std::pair<int, int> x){ return (x.first + x.second) % 2 == 0; });
    auto sl3 = drop(sl2, 1);

    for (auto el : v | reverse_view() | filter_view([](int x){ return x % 2 == 0; }) | take_view(4) | transform_view([](int x){ return x * x; }) | drop_view(2) | reverse_view()) {
        std::cout << el << ' ';
    }
    /*
    for (auto el : v | reverse_view() | reverse_view()) {
        std::cout << el << ' ';
    }
    */
}