#include "deduction_guides.h"

#include <cstddef>

using namespace std;

namespace single
{
    static_assert(is_same_v<decltype(unique_ptr(new int, default_delete<int>())),
                            unique_ptr<int, default_delete<int>>>);

    struct int_deleter : std::default_delete<int>
    {
        using pointer = int*;

        static const int_deleter instance;
    };

    static_assert(is_same_v<decltype(unique_ptr(new int, int_deleter())),
                            unique_ptr<int, int_deleter>>);

    static_assert(is_same_v<decltype(unique_ptr(new int, int_deleter::instance)),
                            unique_ptr<int, int_deleter>>);

    static_assert(is_same_v<decltype(unique_ptr(new int, declval<int_deleter const &&>())),
                            unique_ptr<int, const int_deleter>>);

    static_assert(is_same_v<decltype(unique_ptr(nullptr, int_deleter())),
                            unique_ptr<int, int_deleter>>);

    namespace pointer_check
    {
        struct base
        {
            virtual ~base();
        };

        struct deleter : default_delete<base>
        {
            using pointer = base *;
        };

        using base_ptr = unique_ptr<base, deleter>;

        struct derived : base {};

        base_ptr    b1(static_cast<base *>(new derived), deleter());
        base_ptr    b2(                    new derived,  deleter());

        unique_ptr  b3(static_cast<base *>(new derived), deleter());
        static_assert(is_same_v<decltype(b3), base_ptr>);

        /* If remove `pointer_is_same` check following definition of `b4` will compile,
         * but static_assert will fail.
        unique_ptr  b4(                    new derived,  deleter());
        static_assert(is_same_v<decltype(b4), base_ptr>);
        */
    }

    static_assert(is_same_v<decltype(unique_ptr(malloc(1), free)),
                            unique_ptr<void, void (&)(void *) noexcept>>);
    static_assert(is_same_v<decltype(unique_ptr(static_cast<byte *>(malloc(1)), free)),
                            unique_ptr<byte, void (&)(void *) noexcept>>);
    static_assert(is_same_v<decltype(unique_ptr(malloc(1), &free)),
                            unique_ptr<void, void (*)(void *) noexcept>>);
}

namespace array
{
    static_assert(is_same_v<decltype(unique_ptr(new int[1], default_delete<int[]>())),
                            unique_ptr<int[], default_delete<int[]>>>);

    struct int_deleter : std::default_delete<int[]>
    {
        using pointer = int*;

        static const int_deleter instance;
    };
}

template<>
constexpr bool std::deleter_applicable_to_array<::array::int_deleter> = true;

namespace array
{
    static_assert(is_same_v<decltype(unique_ptr(new int[1], int_deleter::instance)),
                            unique_ptr<int[], int_deleter>>);

    static_assert(is_same_v<decltype(unique_ptr(nullptr, int_deleter())),
                            unique_ptr<int[], int_deleter>>);
}
