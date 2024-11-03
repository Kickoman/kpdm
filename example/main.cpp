#include "kpdm.h"
#include <thread>
#include <vector>
#include <random>
#include <algorithm>


int getRandomInt() {
    static std::random_device randomDevice;
    static std::mt19937 mersenneEngine(randomDevice());
    static std::uniform_int_distribution<int> dist(0, 100);
    return dist(mersenneEngine);
}

int main() {
    using namespace std::literals::chrono_literals;
    std::vector<int> v(20);
    std::generate(v.begin(), v.end(), getRandomInt);

    for (const auto & x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << v.end() - v.begin() << std::endl;

    int sum = 0;
    for (auto &x : kpdm(v)) {
        sum += x;
        x *= 2;
        std::this_thread::sleep_for(100ms);
    }

    for (const auto & x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << sum << std::endl;

    for (const auto i : kpdm(0, 1000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
