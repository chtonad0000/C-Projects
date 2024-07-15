#pragma once

#include <numeric>

namespace algorithm {
    template<typename Iterator> concept InputIterator = requires(Iterator it) {
        typename std::iterator_traits<Iterator>::iterator_category;
        requires std::derived_from<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>;
    };


    template<typename Iterator> concept ForwardIterator = requires(Iterator it) {
        typename std::iterator_traits<Iterator>::iterator_category;
        requires std::derived_from<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>;
    };

    template<typename Iterator> concept BidirectionalIterator = requires(Iterator it) {
        typename std::iterator_traits<Iterator>::iterator_category;
        requires std::derived_from<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>;
    };

    template<InputIterator InputIterator, typename Predicate>
    bool all_of(InputIterator _first, InputIterator _last, Predicate _pred) {
        if (_first == _last) {
            return true;
        }
        auto curr_it = _first;
        while (curr_it != _last) {
            if (!_pred(*curr_it)) {
                return false;
            }
            ++curr_it;
        }

        return true;
    }

    template<InputIterator InputIterator, typename Predicate>
    bool any_of(InputIterator _first, InputIterator _last, Predicate _pred) {
        if (_first == _last) {
            return true;
        }
        auto curr_it = _first;
        while (curr_it != _last) {
            if (_pred(*curr_it)) {
                return true;
            }
            ++curr_it;
        }

        return false;
    }

    template<InputIterator InputIterator, typename Predicate>
    bool none_of(InputIterator _first, InputIterator _last, Predicate _pred) {
        if (_first == _last) {
            return true;
        }
        auto curr_it = _first;
        while (curr_it != _last) {
            if (_pred(*curr_it)) {
                return false;
            }
            ++curr_it;
        }

        return true;
    }

    template<InputIterator InputIterator, typename Predicate>
    bool one_of(InputIterator _first, InputIterator _last, Predicate _pred) {
        if (_first == _last) {
            return false;
        }
        int count = 0;
        auto curr_it = _first;
        while (curr_it != _last) {
            if (_pred(*curr_it)) {
                ++count;
            }
            ++curr_it;
        }
        if (count == 1) {
            return true;
        }

        return false;
    }

    template<ForwardIterator ForwardIterator, typename Compare>
    bool is_sorted(ForwardIterator _first, ForwardIterator _last, Compare _compare) {
        if (_first == _last) {
            return true;
        }
        auto next = _first;
        ++next;
        while (next != _last) {
            if (_compare(*next, *_first)) {
                return false;
            }
            ++_first;
            ++next;
        }

        return true;
    }

    template<ForwardIterator ForwardIterator, typename Predicate>
    bool is_partitioned(ForwardIterator _first, ForwardIterator _last, Predicate _pred) {
        if (_first == _last) {
            return true;
        }
        auto curr_it = _first;
        bool flag = _pred(*_first);
        int change = 0;
        ++curr_it;
        while (curr_it != _last) {
            if (_pred(*curr_it) != flag) {
                ++change;
                if (change == 2) {
                    return false;
                }
                flag = !flag;
            }
            ++curr_it;
        }

        return true;
    }

    template<typename Element, InputIterator InputIterator>
    InputIterator find_not(InputIterator _first, InputIterator _last, Element _elem) {
        if (_first == _last) {
            return _last;
        }
        auto curr_it = _first;
        while (curr_it != _last) {
            if (*curr_it != _elem) {
                return curr_it;
            }
            ++curr_it;
        }

        return _last;
    }

    template<typename Element, InputIterator InputIterator>
    InputIterator find_backward(InputIterator _first, InputIterator _last, Element _elem) {
        if (_first == _last) {
            return _last;
        }
        InputIterator result = _last;
        auto curr_it = _first;
        while (curr_it != _last) {
            if (*curr_it == _elem) {
                result = curr_it;
            }
            ++curr_it;
        }

        return result;
    }

    template<typename Iterator>
    bool is_palindrome(Iterator _first, Iterator _last) {
        if constexpr (BidirectionalIterator<Iterator>) {
            --_last;
            while (_first < _last) {
                if (*_first != *_last) return false;
                ++_first;
                --_last;
            }

            return true;
        } else if constexpr (InputIterator<Iterator>) {
            auto start = _first;
            int size = 0;
            while (start != _last) {
                ++start;
                ++size;
            }
            start = _first;
            for (int i = 0; i < size / 2 - 1; ++i) {
                auto it = _first;
                for (int j = 0; j < size - 1 - i; ++j) {
                    ++it;
                }
                if (*start != *it) {
                    return false;
                }
                ++start;
            }

            return true;
        }
    }
}

