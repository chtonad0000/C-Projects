#include "memory_pool.h"

Pool::Pool(size_t num_blocks, size_t block_size) {
    num_blocks_ = num_blocks;
    block_size_ = block_size;
    pool_ = malloc(num_blocks_ * block_size_);
    free_blocks_ = new bool[num_blocks];
    memset(free_blocks_, true, num_blocks);
    first_block_ = static_cast<char*>(pool_);
}

Pool::Pool(const Pool& other) {
    this->pool_ = malloc(other.block_size_ * other.num_blocks_);
    this->block_size_ = other.block_size_;
    this->num_blocks_ = other.num_blocks_;
    this->free_blocks_ = new bool[other.num_blocks_];
    std::copy(other.free_blocks_, other.free_blocks_ + num_blocks_, this->free_blocks_);
    this->first_block_ = static_cast<char*>(pool_);
}

Pool& Pool::operator=(const Pool& other) {
    this->pool_ = malloc(other.block_size_ * other.num_blocks_);
    this->block_size_ = other.block_size_;
    this->num_blocks_ = other.num_blocks_;
    this->free_blocks_ = new bool[other.num_blocks_];
    std::copy(other.free_blocks_, other.free_blocks_ + num_blocks_, this->free_blocks_);
    this->first_block_ = static_cast<char*>(pool_);

    return *this;
}

bool Pool::operator<(const Pool& other) const {
    if (this->block_size_ < other.block_size_) {
        return true;
    }

    return false;
}

Pool::~Pool() {
    free(pool_);
    delete[] free_blocks_;
}

bool Pool::AllocPossibility(size_t size_of_value) {
    size_t capacity;
    if (size_of_value % block_size_ != 0) {
        capacity = size_of_value / block_size_ + 1;
    } else {
        capacity = size_of_value / block_size_;
    }
    size_t count = 0;
    for (int i = 0; i < num_blocks_ - capacity + 1; ++i) {
        if (capacity >= num_blocks_ + 1) {
            return false;
        }
        if (free_blocks_[i]) {
            ++count;
        } else {
            count = 0;
        }
        if (count == capacity) {
            return true;
        }
    }

    return false;
}

void* Pool::alloc(size_t size_of_value) {
    size_t capacity;
    if (size_of_value % block_size_ != 0) {
        capacity = size_of_value / block_size_ + 1;
    } else {
        capacity = size_of_value / block_size_;
    }
    if (num_blocks_ - last_free_block_ >= capacity) {
        for (size_t k = last_free_block_; k < last_free_block_ + capacity; ++k) {
            free_blocks_[k] = false;
        }
        void* result = static_cast<void*>(first_block_ + (last_free_block_ * block_size_));
        last_free_block_ += capacity;

        return result;
    }
    for (int i = 0; i < num_blocks_ - capacity + 1; ++i) {
        bool empty_space = true;
        for (int j = i; j < i + capacity; ++j) {
            if (!free_blocks_[j]) {
                empty_space = false;
            }
        }
        if (empty_space) {
            for (size_t k = i; k < i + capacity; ++k) {
                free_blocks_[k] = false;
            }

            return static_cast<void*>(first_block_ + (i * block_size_));
        }
    }

    return nullptr;
}

void Pool::dealloc(void* pointer, size_t size_of_value) {
    size_t capacity;
    if (size_of_value % block_size_ != 0) {
        capacity = size_of_value / block_size_ + 1;
    } else {
        capacity = size_of_value / block_size_;
    }
    if (first_block_ + (last_free_block_ * block_size_ - capacity) == pointer) {
        last_free_block_ -= capacity;
    }
    size_t blocks;
    blocks = (static_cast<char*>(pointer) - first_block_) / block_size_;
    for (int i = 0; i < capacity; ++i) {
        free_blocks_[blocks + i] = true;
    }
}

bool Pool::has_pointer(void* p) {
    if (p >= first_block_ && p < first_block_ + num_blocks_ * block_size_) {
        return true;
    }

    return false;
}
