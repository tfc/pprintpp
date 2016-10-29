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
#include "charlist.hpp"

namespace pprintpp {

using namespace typelist;
using namespace charlist;


template <typename T> struct format_for;

template <> struct format_for<char>  { using type = char_tl_t<'c'>; };
template <> struct format_for<int>   { using type = char_tl_t<'d'>; };
template <> struct format_for<float> { using type = char_tl_t<'f'>; };
template <> struct format_for<double> { using type = char_tl_t<'l', 'f'>; };
template <> struct format_for<const char*> { using type = char_tl_t<'s'>; };

template <class InList, class OutList, size_t Counter>
struct find_brace;

template <class InList, class OutList>
struct find_brace<tl<char_t<'}'>, InList>, OutList, 1> {
    using before = OutList;
    using after  = InList;
};
template <class InList, class OutList, size_t N>
struct find_brace<tl<char_t<'}'>, InList>, OutList, N>
    : public find_brace<InList, append_t<OutList, char_t<'}'>>, N - 1>
{};
template <class InList, class OutList, size_t N>
struct find_brace<tl<char_t<'{'>, InList>, OutList, N>
    : public find_brace<InList, append_t<OutList, char_t<'{'>>, N + 1>
{};
template <char C, class InList, class OutList, size_t N>
struct find_brace<tl<char_t<C>, InList>, OutList, N>
    : public find_brace<InList, append_t<OutList, char_t<C>>, N>
{};

template<typename SL, typename TL>
struct autoformat;

template<typename TL>
struct autoformat<null_t, TL> {
    using type = null_t;
};

template<typename SL, typename T, typename TL>
struct autoformat<tl<char_t<'{'>, SL>, tl<T, TL>>
{
    using other_brace  = find_brace<SL, null_t, 1>;
    using format_block = typename other_brace::before;
    using rest_str     = typename other_brace::after;

    using fmt_str = typename format_for<T>::type;

    using type = tl<char_t<'%'>,
                    append_t<fmt_str, typename autoformat<rest_str, TL>::type>>;
};

template<typename C, typename SL, typename TL>
struct autoformat<tl<C, SL>, TL> {
    using type = tl<C, typename autoformat<SL, TL>::type>;
};


template<typename StringProvider, typename PtList>
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

#define pprintf(s, ...) printf(AUTOFORMAT(s, __VA_ARGS__), __VA_ARGS__);

}
