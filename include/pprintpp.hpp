/*
 * MIT License
 *
 * Copyright (c) 2016 Jacek Galowicz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <tuple>
#include <type_traits>
#include "charlist.hpp"

namespace pprintpp {

using namespace typelist;
using namespace charlist;

template <typename T> struct type2fmt;

template <> struct type2fmt<char>                { using type = char_tl_t<'c'>; };
template <> struct type2fmt<short>               { using type = char_tl_t<'d'>; };
template <> struct type2fmt<int>                 { using type = char_tl_t<'d'>; };
template <> struct type2fmt<long int>            { using type = char_tl_t<'l', 'd'>; };
template <> struct type2fmt<long long int>       { using type = char_tl_t<'l', 'l', 'd'>; };
template <> struct type2fmt<unsigned char>       { using type = char_tl_t<'u'>; };
template <> struct type2fmt<unsigned short>      { using type = char_tl_t<'u'>; };
template <> struct type2fmt<unsigned>            { using type = char_tl_t<'u'>; };
template <> struct type2fmt<unsigned long>       { using type = char_tl_t<'l', 'u'>; };
template <> struct type2fmt<unsigned long long>  { using type = char_tl_t<'l', 'l', 'u'>; };

template <> struct type2fmt<float>  { using type = char_tl_t<'f'>; };
template <> struct type2fmt<double> { using type = char_tl_t<'l', 'f'>; };

template <> struct type2fmt<std::nullptr_t> { using type = char_tl_t<'p'>; };
template <typename T> struct type2fmt<T*>   { using type = char_tl_t<'p'>; };

template <typename T, typename FL>
struct format_str {
    static constexpr bool s_fmt {contains<FL, char_t<'s'>>::value};
    static constexpr bool is_str {std::is_same<char,
        typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::value};

    using raw_fmt = typename type2fmt<T>::type;
    using type = typename std::conditional<s_fmt && is_str,
          substitute_t<raw_fmt, char_t<'p'>, char_t<'s'>>,
          raw_fmt
        >::type;
};

template <class InList, class OutList, size_t Counter>
struct find_brace;

template <class InList, class OutList>
struct find_brace<tl<char_t<'}'>, InList>, OutList, 1> {
    using before = OutList;
    using after  = InList;
};

template <char C, class InList, class OutList, size_t N>
struct find_brace<tl<char_t<C>, InList>, OutList, N>
    : public find_brace<InList, append_t<OutList, char_t<C>>, N>
{
    static_assert(C != '{', "Found nested braces: {...{...}...}!"
                            " Maybe you want to mask the outer one?");
};

template <class OutList, size_t N>
struct find_brace<null_t, OutList, N>
{
    static_assert(N + 1 == N, "foooo");
};

template <typename SL, typename TL>
struct autoformat;

template <>
struct autoformat<null_t, null_t> { using type = null_t; };

template <typename TL>
struct autoformat<null_t, TL> { using type = null_t; };

template <typename SL, typename TL>
struct autoformat<tl<char_t<'\\'>, tl<char_t<'{'>, SL>>, TL>
{
    using type = tl<char_t<'{'>, typename autoformat<SL, TL>::type>;
};

template <typename SL, typename TL>
struct autoformat<tl<char_t<'{'>, SL>, TL>
{
    using other_brace  = find_brace<SL, null_t, 1>;
    using format_block = typename other_brace::before;
    using rest_str     = typename other_brace::after;

    static_assert(!std::is_same<TL, null_t>::value, "fooooo");
    using T = typename TL::head;
    using fmt_str = typename format_str<T, format_block>::type;

    using type = tl<char_t<'%'>,
                    append_t<fmt_str, typename autoformat<rest_str, typename TL::tail>::type>>;
};

template <typename C, typename SL, typename TL>
struct autoformat<tl<C, SL>, TL> {
    using type = tl<C, typename autoformat<SL, TL>::type>;
};


template <typename StringProvider, typename PtList>
using autoformat_t =
    tl_to_varlist<
        typename autoformat<string_list_t<StringProvider>, PtList>::type
    >;

template <typename ... Ts>
make_t<Ts...> tie_types(Ts...);

#define AUTOFORMAT(s, ...) \
    ({ \
        struct strprov { static constexpr const char * const str() { return s; } }; \
        using paramtypes = decltype(pprintpp::tie_types(__VA_ARGS__)); \
        using af = pprintpp::autoformat_t<strprov, paramtypes>; \
        af::str(); \
    })

#define pprintf(s, ...) printf(AUTOFORMAT(s, ## __VA_ARGS__), ## __VA_ARGS__);

}
