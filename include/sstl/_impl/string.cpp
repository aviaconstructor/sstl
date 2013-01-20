#include "../string"
#include "../algorithm"
#include "../mutex"

namespace SSTL_NAMESPACE {

string::_buffer_type string::_empty_string_buffer;
string string::_empty_string(&string::_empty_string_buffer);

inline unsigned _adjust_capacity(unsigned size)
{
    unsigned result = (size < string::_minimum_capacity) ? string::_minimum_capacity : size;
    result += 7;
    result &= ~7; // round up to the nearest 8
    return result;
}

void string::reserve(size_type reserved_size)
{
    if (reserved_size > capacity())
        _reallocate(reserved_size);
}

void string::shrink_to_fit()
{
    const size_type new_capacity = _adjust_capacity(size());
    if (new_capacity < capacity())
        _reallocate(new_capacity);
}

const char* string::c_str() const
{
    const size_type s = size();
    if (capacity() == s) // have to reallocate
        _reallocate(s + 1);
    _bytes[s] = '\0'; // if capacity allows, it is safe to do even if there are many references
    return _bytes;
}

// Return the hash value for a zero terminated character string.
//
// The algorithm is loosely based on Jenkins one-at-a-time hash function.
///
unsigned string::static_hash(const char* p, size_type size)
{
    if (size == 0)
        return 1;

    unsigned hash = size;

#define HASH_BYTE   hash += static_cast<unsigned>(static_cast<unsigned char>(*p++)); \
                    hash += hash << 10; \
                    hash ^= hash >> 6;

    unsigned octavas = (size + 7) >> 3;
    switch (size & 7)
    {
    case 0:
        do   // Duff's device super trick in action
        {
            HASH_BYTE
            case 7: HASH_BYTE
            case 6: HASH_BYTE
            case 5: HASH_BYTE
            case 4: HASH_BYTE
            case 3: HASH_BYTE
            case 2: HASH_BYTE
            case 1: HASH_BYTE
        } while ( --octavas != 0 );
    }

#undef HASH_BYTE

    // Final shuffling
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    if (hash == 0)
        ++hash; // hash value should never be zero
    return hash;
}

void string::resize(size_type new_size)
{
    size_type old_size = size();
    if (new_size != old_size)
    {
        if (new_size < old_size) // shrink
        {
            unshare();
            _get_buffer()->_size = new_size;
        }
        else // grow
        {
            const size_type diff = new_size - old_size;
            char* buff = _append_uninitialized(diff);
            memset(buff, 0, diff);
        }
    }
}

string& string::erase(size_type pos, size_type count)
{
    SSTL_ASSERT(pos + count <= size());
    if (count != 0)
    {
        unshare();
        const size_type end_pos = pos + count;
        memmove(_bytes + pos, _bytes + end_pos, size() - end_pos);
        _get_buffer()->_size -= count;
    }
    return *this;
}

int string::compare(const string& s) const
{
    if (_bytes == s._bytes) // fast version
        return 0;
    return compare(s._bytes, size());
}

int string::compare(const char* s) const
{
    size_type len = static_cast<size_type>(strlen(s));
    return compare(s, len);
}

int string::compare(const char* s, size_type len) const
{
    size_type size1 = size();
    int result;
    if (size1 == len)
        result = memcmp(_bytes, s, size1);
    else if (size1 < len)
    {
        result = memcmp(_bytes, s, size1);
        if ( result == 0 )
            result = -1;
    }
    else
    {
        SSTL_ASSERT(size1 > len);
        result = memcmp(_bytes, s, len);
        if ( result == 0 )
            result = 1;
    }
    return result;
}

bool string::operator==(const string& s) const
{
    if (_bytes == s._bytes)
        return true;
    const _buffer_type* b1 = _get_buffer();
    const _buffer_type* b2 = s._get_buffer();
    if ( b1->_size != b2->_size )
        return false;
    return memcmp(_bytes, s._bytes, b1->_size) == 0;
}

bool string::operator==(const char* s) const
{
    size_type len = static_cast<size_type>(strlen(s));
    if (len != size())
        return false;
    return memcmp(_bytes, s, len) == 0;
}

string::size_type string::find(char ch, size_type pos) const
{
   const_iterator i = cbegin() + pos;
   const_iterator i_end = cend();
   for ( ; i < i_end; ++i) // when pos >= size() return npos, as specified
      if (*i == ch)
         return i - cbegin();
   return npos;
}

string::size_type string::find(const char* s, size_type pos, size_type count) const
{
    SSTL_ASSERT(s != NULL);
    size_type len = size();
    if (len - pos >= count && pos <= len)
    {
        if (count == 0) // this has to happen after the above if
            return pos;
        const char* d = data();
        const char* d_end = data() + len;
        const char* f = SSTL_NAMESPACE::search(d + pos, d_end, s, s + count);
        if (f < d_end)
            return static_cast<string::size_type>(f - d);
    }
    return npos;
}

string::size_type string::rfind(char ch, size_type pos) const
{
    size_type len = size();
    if (len != 0)
    {
        const char* d = data();
        const char* i = d + (pos < len ? pos : len - 1);
        for (; i >= d; --i)
            if (*i == ch)
                return static_cast<string::size_type>(i - d);
    }
    return npos;
}

string::size_type string::rfind(const char* s, size_type pos, size_type count) const
{
    SSTL_ASSERT(s != NULL);
    size_type len = size();
    if (pos < len && len - pos >= count)
        pos += count;
    else
        pos = len;
    if (count == 0)
        return pos;

    const char* d = data();
    const char* d_end = d + pos;
    const char* f = SSTL_NAMESPACE::find_end(d, d_end, s, s + count);
    if (f < d_end)
        return static_cast<string::size_type>(f - d);
    return npos;
}

void string::_reallocate(size_type new_capacity) const
{
    SSTL_ASSERT(size() <= new_capacity);

    new_capacity = _adjust_capacity(new_capacity);
    _buffer_type* buff = _new_uninitialized_buffer(size(), new_capacity);
    memcpy(buff->_bytes, _bytes, buff->_size);
    _get_buffer()->_ref_decrement();
    _bytes = buff->_bytes;
}

string::_buffer_type* string::_new_uninitialized_buffer(size_type size, size_type capacity)
{
    SSTL_ASSERT(size <= capacity);
    SSTL_ASSERT(capacity >= _minimum_capacity);
    unsigned buffer_sizeof = _buffer_type_header_sizeof + capacity;
    _buffer_type* buff = reinterpret_cast<_buffer_type*>(new char[buffer_sizeof]);
    buff->_hash = 0;
    buff->_capacity = capacity;
    buff->_size = size;
    buff->_ref_count = 0;
    return buff;
}

char* string::_new_uninitialized(size_type size)
{
    return _new_uninitialized_buffer(size, _adjust_capacity(size))->_bytes;
}

void string::unshare()
{
    _buffer_type* buff = _get_buffer();
    if (buff->_ref_count > 0)
    {
        char* bytes = _new_uninitialized(buff->_size);
        memcpy(bytes, _bytes, buff->_size);
        _get_buffer()->_ref_decrement();
        _bytes = bytes;
    }
    else
    {
        SSTL_ASSERT(!is_interned()); // attempt to modify a non-referenced interned string is made somehow
    }
}

char* string::_append_uninitialized(size_type count)
{
    SSTL_ASSERT(!is_interned()); // attempt to modify a readonly interned string is made
    size_type old_size = size();
    size_type new_size = old_size + count;
    if (new_size < capacity()) // +1 for terminating zero
        unshare();
    else // grow
    {
        char* bytes = _new_uninitialized(new_size);
        memcpy(bytes, _bytes, old_size);
        #if SSTL_DEBUG
            memset(bytes + oldSize, 0, newSize - oldSize);
        #endif
        _get_buffer()->_ref_decrement();
        _bytes = bytes;
    }
    _get_buffer()->_size = new_size;
    return _bytes + old_size;
}

char* string::_insert_uninitialized(size_type index, size_type count)
{
    SSTL_ASSERT(!is_interned()); // attempt to modify a readonly interned string is made
    SSTL_ASSERT(index <= size());
    SSTL_ASSERT(count != npos);
    size_type old_size = size();
    size_type new_size = old_size + count;
    if (new_size < capacity()) // +1 for terminating zero
    {
        unshare();
        memmove(_bytes + index + count, _bytes + index, old_size - index);
    }
    else // grow
    {
        char* bytes = _new_uninitialized(new_size);
        memcpy(bytes, _bytes, index);
        #if SSTL_DEBUG
            memset(bytes + index, 0, count);
        #endif
        memcpy(bytes + index + count, _bytes + index, old_size - index);
        _get_buffer()->_ref_decrement();
        _bytes = bytes;
    }
    _get_buffer()->_size = new_size;
    return _bytes + old_size;
}

char* string::_replace_uninitialized(size_type pos, size_type count, size_type new_count)
{
    SSTL_ASSERT(!is_interned()); // attempt to modify a readonly interned string is made
    SSTL_ASSERT(pos <= size());
    SSTL_ASSERT(pos + count <= size());
    if (new_count < count) // shrink
        erase(pos + new_count, count - new_count);
    else if (new_count > count) // grow
        _insert_uninitialized(pos + count, new_count - count);
    return _bytes + pos;
}

// Support for string interning

class _intern_holder
{
public: // Constants:

    // Hash cell index decrement to find the next cell
    //
    static const int hashtable_secondary_shift = 1;

    // Default size of the hash table
    //
    static const int hashtable_default_size = 1024;

public:

    _intern_holder()
        :
          _capacity(0),
          _count(0),
          _buffers(NULL)
    {}

    ~_intern_holder()
    {
        lock_guard<mutex> lock(_lock);
        string::_buffer_type** it = _buffers;
        string::_buffer_type** itEnd = _buffers + _capacity;
        for ( ; it != itEnd; ++it )
            if ( *it != NULL )
                (*it)->_ref_decrement();
        delete [] _buffers;
    }

    void add(string& str)
    {
        string::_buffer_type* buff = str._get_buffer();
        if (buff->_size == 0)
        {
            if (buff != &string::_empty_string_buffer)
            {
                buff->_ref_decrement();
                str._bytes = string::_empty_string_buffer._bytes;
                string::_empty_string_buffer._ref_increment();
            }
            return;
        }
        SSTL_ASSERT(buff->_hash == 0); // otherwise we would not be here
        buff->_hash = string::static_hash(buff->_bytes, buff->_size);

        lock_guard<mutex> lock(_lock);
        string::_buffer_type** cell = find_cell_for_addition(buff->_hash, buff->_bytes, buff->_size);
        if (*cell != NULL)
        {
            buff->_ref_decrement();
            str._bytes = (*cell)->_bytes;
        }
        else
            *cell = buff;
        (*cell)->_ref_increment();
    }

    string::_buffer_type* add(const char* str, unsigned size)
    {
        if (size == 0)
        {
            string::_empty_string_buffer._ref_increment();
            return &string::_empty_string_buffer; // special value, always interned
        }

        unsigned hash = string::static_hash(str, size);

        lock_guard<mutex> lock(_lock);
        string::_buffer_type** cell = find_cell_for_addition(hash, str, size);
        if (*cell == NULL)
        {
            string::_buffer_type* buff = string::_new_uninitialized_buffer(size, _adjust_capacity(size));
            memcpy(buff->_bytes, str, size);
            buff->_hash = hash;
            *cell = buff;
        }

        (*cell)->_ref_increment();
        return *cell;
    }

    void OptimizeAndGarbageCollect()
    {
        if (_count != 0)
        {
            lock_guard<mutex> lock(_lock);
            resize(_capacity);
        }
    }

    void resize(int new_capacity);

    string::_buffer_type** find_cell_for_addition(unsigned hash, const char* bytes, unsigned size);

    static _intern_holder* get_global()
    {
        static _intern_holder holder;
        return &holder;
    }

private:

    int _capacity; // has to be power of two
    int _count;
    string::_buffer_type** _buffers;
    sstl::mutex _lock;
};

string::_buffer_type** _intern_holder::find_cell_for_addition(unsigned hash, const char* bytes, unsigned size)
{
    if (_capacity <= (_count << 1))
        resize(_capacity == 0 ? hashtable_default_size : _capacity + _capacity);

    int index = static_cast<int>(hash & (_capacity - 1u)); // normalize hash into index
    string::_buffer_type** bb;
    for (;;)
    {
        bb = &_buffers[index];
        const string::_buffer_type* b = *bb;
        if ( b == NULL )
        {
            ++_count;  // we know there will be a new item
            return bb;  // empty place to add the new item
        }
        if ( b->_hash == hash && b->_size == size && memcmp(b->_bytes, bytes, size) == 0 )
            return bb;  // the same item is found

        // Otherwise calculate the second-grade hash value, derivative from one given
        index -= hashtable_secondary_shift;
        if ( index < 0 ) // assume proper overflow behavior...
            index += _capacity;
    }
}

void _intern_holder::resize(int new_capacity)
{
    SSTL_ASSERT(new_capacity >= _capacity);
    SSTL_ASSERT((new_capacity & (new_capacity - 1)) == 0); // newCapacity is the power of two

    string::_buffer_type** new_buffers = new string::_buffer_type*[new_capacity];

    string::_buffer_type** i = new_buffers;
    string::_buffer_type** i_end = new_buffers + new_capacity;
    for (; i != i_end; ++i)
        *i = NULL;

    if (_count != 0)
    {
        int new_count = 0;
        i = _buffers;
        i_end = _buffers + _capacity;
        for (; i != i_end; ++i)
        {
            string::_buffer_type* buff = *i;
            if (buff != NULL)
            {
                SSTL_ASSERT(buff->_hash != 0);
                if (buff->_ref_count == 0) // orphaned item to garbage collect
                    delete buff;
                else
                {
                    SSTL_ASSERT(buff->_ref_count > 0);

                    int index = static_cast<int>(buff->_hash & (_capacity - 1u)); // normalize hash into index
                    string::_buffer_type** bb;
                    for (;;)
                    {
                        bb = &_buffers[index];
                        const string::_buffer_type* b = *bb;
                        if (b == NULL)
                        {
                            *bb = buff; // relocate
                            ++new_count;
                            break;
                        }

                        // Otherwise calculate the second-grade hash value, derivative from one given
                        index -= hashtable_secondary_shift;
                        if ( index < 0 ) // assume proper overflow behavior...
                            index += _capacity;
                    }
                }
            }
        }
        SSTL_ASSERT(_count >= new_count);
        _count = new_count;
    }
    delete _buffers;
    _buffers = new_buffers;
    _capacity = new_capacity;
}

void string::intern()
{
    if (!is_interned())
        _intern_holder::get_global()->add(*this);
}

string string::intern_create(const char* s)
{
    return _intern_holder::get_global()->add(s, strlen(s));
}

string string::intern_create(const char* s, size_type size)
{
    return _intern_holder::get_global()->add(s, size);
}

void string::intern_cleanup(time_t secondsSincePrevious)
{
    if ( secondsSincePrevious > 0 )
    {
        static time_t s_lastTime = 0; // time in the past
        time_t now = time(NULL);
        if ( now - s_lastTime < secondsSincePrevious )
            return;
        s_lastTime = now;
    }
    _intern_holder::get_global()->OptimizeAndGarbageCollect();
}

}
