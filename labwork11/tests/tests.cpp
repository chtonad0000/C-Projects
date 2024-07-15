#include "algorithms.h"
#include "xrange.h"
#include "zip.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <list>

TEST(AlgorithmsTests, all_of) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<std::string> l = {"2", "2", "2"};

    ASSERT_FALSE(algorithm::all_of(v.begin(), v.end(), [](int i) {
        return i % 2 == 0;
    }));
    ASSERT_TRUE(algorithm::all_of(l.begin(), l.end(), [](std::string i) {
        return i == "2";
    }));
}

TEST(AlgorithmsTests, any_of) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<std::string> l = {"2", "2", "2"};

    ASSERT_TRUE(algorithm::any_of(v.begin(), v.end(), [](int i) {
        return i % 2 == 0;
    }));
    ASSERT_FALSE(algorithm::any_of(l.begin(), l.end(), [](std::string i) {
        return i == "1";
    }));
}

TEST(AlgorithmsTests, none_of) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<std::string> l = {"2", "2", "2"};

    ASSERT_TRUE(algorithm::none_of(v.begin(), v.end(), [](int i) {
        return i % 10 == 0;
    }));
    ASSERT_TRUE(algorithm::none_of(l.begin(), l.end(), [](std::string i) {
        return i == "1";
    }));
}

TEST(AlgorithmsTests, one_of) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::list<std::string> l = {"2", "2", "2"};

    ASSERT_TRUE(algorithm::one_of(v.begin(), v.end(), [](int i) {
        return i % 10 == 0;
    }));
    ASSERT_FALSE(algorithm::one_of(l.begin(), l.end(), [](std::string i) {
        return i == "2";
    }));
}

TEST(AlgorithmsTests, is_sorted) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<int> l = {1, 2, 5, 3, 7, 3, 74, 7, 4, 7};

    ASSERT_TRUE(algorithm::is_sorted(v.begin(), v.end(), std::less<int>()));
    ASSERT_FALSE(algorithm::is_sorted(l.begin(), l.end(), std::less<int>()));
}

TEST(AlgorithmsTests, is_partitionded) {
    std::vector<int> v = {1, 1, 1, 1, 5, 6, 7, 8, 9};
    std::vector<int> v2 = {1, 2, 5, 3, 7, 3, 74, 7, 4, 7};

    ASSERT_TRUE(algorithm::is_partitioned(v.begin(), v.end(), [](int i) {
        return i == 1;
    }));
    ASSERT_FALSE(algorithm::is_partitioned(v2.begin(), v2.end(), [](int i) {
        return i % 2 == 0;
    }));
}

TEST(AlgorithmsTests, find_not) {
    std::vector<int> v = {1, 1, 1, 1, 5, 6, 7, 8, 9};
    std::list<int> l = {1, 2, 5, 3, 7, 3, 74, 7, 4, 7};

    ASSERT_EQ(*algorithm::find_not(v.begin(), v.end(), 1), 5);
    ASSERT_EQ(*algorithm::find_not(l.begin(), l.end(), 2), 1);
}

TEST(AlgorithmsTests, find_backward) {
    std::vector<int> v = {1, 1, 1, 1, 5, 6, 7, 8, 9};
    std::list<int> l = {1, 2, 5, 3, 7, 3, 74, 7, 7, 7};

    ASSERT_EQ(algorithm::find_backward(v.begin(), v.end(), 8), v.end() - 2);
    ASSERT_EQ(*algorithm::find_backward(l.begin(), l.end(), 74), 74);
}

TEST(AlgorithmsTests, is_palindrome) {
    std::vector<int> v = {1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<int> v2 = {1, 2, 5, 3, 7, 3, 5, 2, 1};
    std::forward_list fl = {0, 1, 2, 3, 2, 1, 0};
    std::forward_list fl2 = {0, 1, 6, 3, 6, 3, 2, 1, 0};

    ASSERT_TRUE(algorithm::is_palindrome(v.begin(), v.end()));
    ASSERT_TRUE(algorithm::is_palindrome(v2.begin(), v2.end()));
    ASSERT_TRUE(algorithm::is_palindrome(fl.begin(), fl.end()));
    ASSERT_FALSE(algorithm::is_palindrome(fl2.begin(), fl2.end()));
}

TEST(XrangeTest, xrange) {
    auto x = xrange(1.5, 5.5);
    std::vector<double> v{x.begin(), x.end()};
    std::vector<double> v_check = {1.5, 2.5, 3.5, 4.5};
    ASSERT_EQ(v, v_check);
    auto y = xrange(4);
    std::vector<int> v2{y.begin(), y.end()};
    std::vector<int> v_check2 = {0, 1, 2, 3};

    ASSERT_EQ(v2, v_check2);
    std::string check;
    for (auto i: xrange(1, 6, 2)) {
        check += std::to_string(i);
    }

    ASSERT_EQ(check, "135");
}

TEST(ZipTest, zip) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    std::string result;
    for (auto value: zip(l, v)) {
        result += std::to_string(value.first) + value.second;
    }

    ASSERT_EQ(result, "1a2b3c4d");
    std::forward_list<int> fl = {8, 6, 3, 0, 9};
    std::set s = {'b', 'a', 'd', 'c'};
    result = "";
    for (auto value: zip(fl, s)) {
        result += std::to_string(value.first) + value.second;
    }

    ASSERT_EQ(result, "8a6b3c0d");
}