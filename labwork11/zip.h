#pragma once

#include <iterator>

template<typename Iterator1, typename Iterator2>
class ZipIterator {
public:
    using value_type = std::pair<typename std::iterator_traits<Iterator1>::value_type,typename std::iterator_traits<Iterator2>::value_type>;
    using reference = value_type&;
    using pointer = value_type*;

    ZipIterator(Iterator1 curr_first, Iterator1 end_first, Iterator2 curr_second, Iterator2 end_second) : curr_first_(
            curr_first), end_first_(end_first), curr_second_(curr_second), end_second_(end_second) {
        if (curr_first_ != end_first_) {
            curr_pair_ = std::make_pair(*curr_first_, *curr_second_);
        }
    }

    reference operator*() {
        return curr_pair_;
    }

    pointer operator->() const {
        return &curr_pair_;
    }

    ZipIterator& operator++() {
        ++curr_first_;
        ++curr_second_;
        if (curr_first_ == end_first_) {
            curr_second_ = end_second_;

            return *this;
        }
        if (curr_second_ == end_second_) {
            curr_first_ = end_first_;

            return *this;
        }
        curr_pair_ = std::make_pair(*curr_first_, *curr_second_);

        return *this;
    }

    ZipIterator operator++(int) {
        auto tmp = *this;
        ++(*this);

        return tmp;
    }

    bool operator==(const ZipIterator& other) const {
        return curr_first_ == other.curr_first_ && curr_second_ == other.curr_second_;
    }

    bool operator!=(const ZipIterator& other) const {
        return !(*this == other);
    }

private:
    Iterator1 curr_first_;
    Iterator1 end_first_;
    Iterator2 curr_second_;
    Iterator2 end_second_;
    value_type curr_pair_;
};


template<typename container_first, typename container_second>
class zip {
public:
    using value_type = std::pair<typename container_first::value_type, typename container_second::value_type>;
    using iterator = ZipIterator<typename container_first::iterator, typename container_second::iterator>;
    using const_iterator = const ZipIterator<typename container_first::iterator, typename container_second::iterator>;

    zip(container_first& data_first, container_second& data_second) : data_first_(data_first),
                                                                      data_second_(data_second) {
    }

    iterator begin() const {
        return ZipIterator<typename container_first::iterator, typename container_second::iterator>(data_first_.begin(),
                                                                                                    data_first_.end(),
                                                                                                    data_second_.begin(),
                                                                                                    data_second_.end());
    }

    iterator end() const {
        return ZipIterator<typename container_first::iterator, typename container_second::iterator>(data_first_.end(),
                                                                                                    data_first_.end(),
                                                                                                    data_second_.end(),
                                                                                                    data_second_.end());
    }

private:
    container_first& data_first_;
    container_second& data_second_;
};