/*
#include "../allocator"
#include "../algorithm"

namespace SSTL_NAMESPACE {

void allocator_base::operator=(const allocator_base& v)
{
    if (&v != this)
    {
        _delete_buffer();
        _buffer = new char[v._size];
        _size = v._size;
        _capacity = _size;
        memcpy(_buffer, v._buffer, _size);
    }
}

bool allocator_base::operator==(const allocator_base& v) const
{
    if (&v == this)
        return true;
    return memcmp(_buffer, v._buffer, _size) == 0;
}

void allocator_base::reserve(sstl_size_type n)
{
    if (n > _capacity)
    {
        char* b = new char[n];
        memcpy(b, _buffer, _size);
        _delete_buffer();
        _buffer = b;
    }
    _capacity = n;
}

void allocator_base::shrink_to_fit()
{
    if (_size < _capacity)
    {
        char* b = new char[_size];
        memcpy(b, _buffer, _size);
        _delete_buffer();
        _buffer = b;
        _capacity = _size;
    }
}

char* allocator_base::insert(sstl_size_type n, sstl_size_type count)
{
    sstl_size_type new_size = _size + count;
    if (new_size > _capacity)
    {
        char *b = new char[new_size];
        _capacity = new_size;
        memcpy(b, _buffer, n);
        memcpy(b + n + count, _buffer + n, _size - count);
    }
    else
        memmove(_buffer + n + count, _buffer + n, _size - count);
    _size = new_size;
}

void allocator_base::erase(sstl_size_type n, sstl_size_type count)
{
    SSTL_ASSERT(n + count <= _size);
    memmove(_buffer + n, _buffer + n + count, _size - (n + count));
}

char* allocator_base::grow(sstl_size_type n)
{

}

void allocator_base::swap(allocator_base& other)
{
    // Also works for
    SSTL_NAMESPACE::swap(other._buffer, _buffer);
    SSTL_NAMESPACE::swap(other._size, _size);
    SSTL_NAMESPACE::swap(other._capacity, _capacity);
}

} // namespace
*/
