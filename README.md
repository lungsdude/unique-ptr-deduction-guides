# Deduction guides for `std::unique_ptr`

[deduction_guides.h](deduction_guides.h) &mdash; deduction guides with some helper metafunctions.
[examples.cpp](examples.cpp) &mdash; examples on how it is meant to be used.

## How it works
If you provide a deleter, the compiler trusts you that you provided the right deleter and sets the type of the unique ptr either the type of the pointer or the type of the first argument of the deleter.
No deduction is provided if no deleter is given (i.e `std::unique_ptr{ new Foo{} };` and `std::unique_ptr{ new Foo[N]{} }`). It will give a compile-time error, because you cannot make a difference between a pointer and a C array - both are pointers. So use `std::make_unique<Foo>()` instead.
