# Deduction guides for `std::unique_ptr`

[deduction_guides.h](deduction_guides.h) &mdash; deduction guides with some helper metafunctions.

[test.cpp](test.cpp) &mdash; compile time tests.

### Notes
* New customization point `template<class> constexpr bool deleter_applicable_to_array` introduced. By default it is equal to `true` for `default_delete<T[]>` and is equal to `false` for everything else. It is necessary for deducing first template argument of `unique_ptr` as `T[]`. Possible alternative is to require from deleters inner typedef `applicable_to_array` (similarly to `is_transparent` for comparators).
* Deleter type is never deduced as a reference except for function types.
* For the deduced type `UP` following invariant holds
  * type `UP::deleter_type::pointer` is ill formed or
  * `is_same<UP::element_type, pointer_traits<UP::deleter_type::pointer>::element_type`>.
