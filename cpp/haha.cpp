#include <iostream>

int main(void) {
    int v[] {0, 1, 2, 3, 4, 5, 6, 7, 8};
    for (auto i: v) {
        std::cout << i << "\n";
    }
    return 0;
}
