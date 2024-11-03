#include "kpdm.h"
#include <thread>

int main() {
    for (const auto i : kpdm(0, 1000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
