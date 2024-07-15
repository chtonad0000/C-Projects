#include "lib/parser.h"

using namespace omfl;

int main(int, char**) {
    std::string data =R"(
        [level1]
        key2 = 2
        [level1.level2.level3]
        key1 = 1)";

    const auto root = parse(data);

    std::cout << root.Get("level1").Get("key2").AsInt() ;
    return 0;
}
