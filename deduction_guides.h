#include <memory>

namespace std
{
    namespace detail
    {
        template<class D, typename = typename D::pointer>
        constexpr bool pointer_is_defined_impl(int)
        {
            return true;
        }

        template<class D>
        constexpr bool pointer_is_defined_impl(...)
        {
            return false;
        }

        template<class D>
        constexpr bool pointer_is_defined = pointer_is_defined_impl<D>(0);

        template<class T, class D, bool = pointer_is_defined<D>>
        struct pointer_is_same;

        template<class T, class D>
        struct pointer_is_same<T, D, false>
                : std::true_type
        {};

        template<class T, class D>
        struct pointer_is_same<T, D, true>
                : std::is_same<T *, typename D::pointer>
        {};
    }

    // new customization point
    template<class>
    constexpr bool deleter_applicable_to_array = false;

    template<class T>
    constexpr bool deleter_applicable_to_array<default_delete<T[]>> = true;

    // [unique.ptr.single.ctor] p9
    template<class T, class D, typename = enable_if_t<
                 is_object_v<D> &&
                 detail::pointer_is_same<T, D>::value &&
                 !deleter_applicable_to_array<D>
            >>
    unique_ptr(T*, const D &) -> unique_ptr<T, D>;

    template<class T, class D, typename = enable_if_t<
                 is_object_v<D> &&
                 detail::pointer_is_same<T, D>::value &&
                 !deleter_applicable_to_array<D>
            >>
    unique_ptr(T*, D &&) -> unique_ptr<T, D>;

    template<class D, typename = enable_if_t<
                 detail::pointer_is_defined<D> &&
                 !deleter_applicable_to_array<D>
            >>
    unique_ptr(nullptr_t, D &&) -> unique_ptr<typename pointer_traits<typename D::pointer>::element_type, D>;

    // [unique.ptr.single.ctor] p10
    template<class T, class D, typename = enable_if_t<is_function_v<D>>>
    unique_ptr(T*, D &) -> unique_ptr<T, D &>;

    // [unique.ptr.runtime.ctor] p2
    template<class T, class D, typename = enable_if_t<
                 is_object_v<D> &&
                 detail::pointer_is_same<T, D>::value &&
                 deleter_applicable_to_array<D>
            >>
    unique_ptr(T*, const D &) -> unique_ptr<T[], D>;

    template<class T, class D, typename = enable_if_t<
                 is_object_v<D> &&
                 detail::pointer_is_same<T, D>::value &&
                 deleter_applicable_to_array<D>
            >>
    unique_ptr(T*, const D &) -> unique_ptr<T[], D>;

    template<class D, typename = enable_if_t<
                 detail::pointer_is_defined<D> &&
                 deleter_applicable_to_array<D>
            >>
    unique_ptr(nullptr_t, D &&) -> unique_ptr<typename pointer_traits<typename D::pointer>::element_type [], D>;
}
