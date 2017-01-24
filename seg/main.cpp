
#include <iostream>
#include <cstdio>

#include <stl_symbols.hpp>
#include <charlist.hpp>

namespace seg {

using namespace charlist;

template <typename inlist, typename current>
struct seg;

template <typename inlist, typename current>
struct seg<tl<char_t<'|'>, inlist>, current>
{
    using current_str = tl_to_varlist<current>;
    using type = tl<current_str, typename seg<inlist, null_t>::type>;
};

template <typename current>
struct seg<null_t, current>
{
    using current_str = tl_to_varlist<current>;
    using type = tl<current_str, null_t>;
};

template <typename c, typename inlist, typename current>
struct seg<tl<c, inlist>, current>
{
    using type = typename seg<inlist, append_t<current, c>>::type;
};

}

// Put type specific print functions here:
///// put_overloaded(int);
///// put_overloaded(float);
///// put_overloaded(whatever);

template <typename chunks>
struct print_seg;

template <typename chunk, typename chunks>
struct print_seg<typelist::tl<chunk, chunks>> {
    template <typename T, typename ... Ts>
    static void print(T t, Ts ... ts) {
        fputs(chunk::str(), stdout);  // puts(str) alone does a newline...
        std::cout << "{" << t << "}"; // put_overloaded(specific_type)
        print_seg<chunks>::print(ts...);
    }

    static void print() {
        std::cout << chunk::str();
        print_seg<chunks>::print();
    }
};

template <>
struct print_seg<typelist::null_t> {
    template <typename T>
    static void print(T t) {
        std::cout << "param {" << t << "}\n";
    }
    static void print() {}
};

#define segprintf(s, ...) \
    ({ \
        struct strprov { static constexpr const char * const str() { return s; } }; \
        using sl = charlist::string_list_t<strprov>; \
        using segpr = print_seg<typename seg::seg<sl, charlist::null_t>::type>; \
        segpr::print( __VA_ARGS__ ); \
    })

int main()
{
    struct strprov {
        static constexpr const char * const str() {
            return "first chunk|second chunk|third chunk";
        }
    };

    segprintf("some int: | some float: | some other str: |\n", 123, 4.56, "foobar");
}
