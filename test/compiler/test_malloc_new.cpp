// Investigate irregularities and approximate overhead
// of allocating small chunks of memory with malloc or new
//

#include <stdio.h> // use libraries that should not allocate memory for IO
#include <malloc.h>
#include <algorithm>
#include <limits>

const int num_allocations = 16;
const size_t minimum_size = 8;
const size_t maximum_size = 0x2000;

template
    <typename alloc_func, typename free_func>
size_t test(const char* name, size_t size, ptrdiff_t& prev_diff, size_t& prev_size, alloc_func allocate, free_func deallocate)
{
    char* a[num_allocations];              // avoid using vector as it allocates
    ptrdiff_t diff = std::numeric_limits<ptrdiff_t>::max();

    // Allocate all at once
    for (int i = 0; i < num_allocations; ++i)
        a[i] = static_cast<char*>(allocate(size));

    // Then deallocate all at once in a separate loop, keep pointer values for analysis
    for (int i = 0; i < num_allocations; ++i)
    {
        if (i > 0)
        {
            ptrdiff_t currDiff = std::abs(a[i - 1] - a[i]);
            if (diff > currDiff)
                diff = currDiff;
        }
        deallocate(a[i]);
    }
    //printf("%d ", diff);
    if (prev_diff == 0)
    {
        prev_diff = diff;
        prev_size = size;
    }
    else if (prev_diff != diff)
    {
        printf("%s: allocalion of %d .. %d bytes takes out %d bytes\n",
               name, static_cast<int>(prev_size), static_cast<int>(size - minimum_size), static_cast<int>(prev_diff));
        prev_diff = diff;
        prev_size = size;
    }
    return diff;
}

static void* allocate_new(size_t s)
{
    return new char[s];
}

static void deallocate_new(char* p)
{
    delete[] p;
}

int main()
{
    ptrdiff_t prev_diff_malloc = 0;
    size_t prev_size_malloc = 0;
    ptrdiff_t prev_diff_new = 0;
    size_t prev_size_new = 0;
    for (size_t size = minimum_size; size <= maximum_size; size += minimum_size)
    {
        test("malloc", size, prev_diff_malloc, prev_size_malloc, malloc, free);
        test("new   ", size, prev_diff_new, prev_size_new, allocate_new, deallocate_new);
    }
    return 0;
}
