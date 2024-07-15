#include <iostream>
#include <algorithm>
#include <list>
#include <forward_list>
#include <set>
#include "algorithms.h"
#include "xrange.h"
#include "zip.h"


int main() {


    std::vector l = {1, 2, 3, 4, 5};
    std::forward_list fl = {0, 1, 2, 3, 2, 1, 0};
    std::set list = {1, 2, 3, 2, 1};
    std::vector v = {'a', 'b', 'c', 'd', 'e'};
    int* a=0;
    std::cout<<algorithm::is_palindrome(fl.begin(),fl.end());
}
