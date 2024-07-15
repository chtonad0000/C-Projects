#include "CCirtucalBuffer.h"
#include "CCirtucalBufferExt.h"
#include <vector>

int main() {
    CCirtucalBufferExt<int> buffer1 = {1, 2, 3, 4, 5, 6};
    buffer1.push_back(8);
    buffer1.push_back(7);
    buffer1 = {1, 2};
    CCirtucalBufferExt<int> res1 = {2, 3, 4, 5, 6, 7};
    std::cout << (buffer1 == res1) << '\n';
    //t2.insert(t2.cbegin(), {1, 2, 3, 4});
    //t2.pop_back();
//    test.push_front(9);
//    test.push_front(8);

    // test.assign(t2.begin(),t2.end());
    //std::cout << t2.begin().current_index_ << '\n';
    //std::cout << test.erase(test.cend() - 4, test.cend() - 1).current_index_ << '\n';
    for (int i = 0; i < buffer1.size(); i++) {
        std::cout << buffer1[i] << ' ';

    }
}