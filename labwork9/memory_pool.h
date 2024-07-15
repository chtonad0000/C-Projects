#pragma once

#include <cstring>
#include <memory>

class Pool {
public:
    Pool(size_t num_blocks, size_t block_size);

    Pool(Pool const& other);

    Pool& operator=(const Pool& other);

    bool operator<(const Pool& other) const;

    ~Pool();

    bool AllocPossibility(size_t size_of_value);

    void* alloc(size_t size_of_value);

    void dealloc(void* pointer, size_t size_of_value);

    bool has_pointer(void* p);

private:
    size_t last_free_block_ = 0;
    size_t num_blocks_;
    size_t block_size_;
    void* pool_;
    char* first_block_;
    bool* free_blocks_;
};