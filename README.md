sstl
====

*** work in progress, no release yet ***

Small Subset of STL C++ library made to solve a very specific problem
in device programming for Internet of Things.
Currently the library only implements the string type,
which is to replace the standard basic_string of char.
This library can be used together with any standard STL implementation, or alone.
It is open sourced here in hopes to be handy for others.

### When to use and when not to

Please consider this library only if standard STL implementations
show unsatisfactory memory consumption in these conditions:
 * you have small amount of memory
 * but you need to deal with lots and lots of strings
 * many of which appear to have same values

If, for example, among all your many strings there are only 20% unique values,
this library allows you save about 60-80% of memory allocated by strings (overhead will vary).

### Implementation detail

As some other basic_string implementations, sstl strings share the same buffer,
which immediately helps the case when the same string is copied many times from the same instance.
To save memory in cases when the strings arrive with data files and through the network,
and merge string duplicates together, a feature known in Java as intern() is implemented.
The programmer can decide which strings should be interned,
and those will be stored n a global internal hash table.
There is also a way of periodically purging strings that are present only in this hash table,
which is a sort of garbage collection,
except that this operation has to be explicitly initiated by the user at desired periods of time.

This all arrives at a price, as such implementation cannot be made fully compatible with the standard.
