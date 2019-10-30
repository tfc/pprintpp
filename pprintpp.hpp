#ifndef _INC_PPRINTPP_
#define _INC_PPRINTPP_

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

#ifndef PPRINTPP_AVOID_STL
#define PPRINTPP_AVOID_STL 1
#pragma message("PPRINTPP MSVC *NOT* using STD lib and *NOT* throwning exceptions\n\n")
#else
#pragma message("PPRINTPP MSVC *using* STD lib and *throwning* exceptions\n\n")
#endif

#include "stl_symbols.hpp"
#include "charlist.hpp"

namespace jacek_galowicz {
	namespace pprintpp {

		constexpr auto VERSION = "0.1.0";

		template <typename T> struct always_false { static constexpr bool value{ false }; };

		using namespace typelist;
		using namespace charlist;

		template <typename T> struct type2fmt;

		template <> struct type2fmt<char> { using type = char_tl_t<'c'>; };
		template <> struct type2fmt<short> { using type = char_tl_t<'d'>; };
		template <> struct type2fmt<int> { using type = char_tl_t<'d'>; };
		template <> struct type2fmt<long int> { using type = char_tl_t<'l', 'd'>; };
		template <> struct type2fmt<long long int> { using type = char_tl_t<'l', 'l', 'd'>; };
		template <> struct type2fmt<unsigned char> { using type = char_tl_t<'u'>; };
		template <> struct type2fmt<unsigned short> { using type = char_tl_t<'u'>; };
		template <> struct type2fmt<unsigned> { using type = char_tl_t<'u'>; };
		template <> struct type2fmt<unsigned long> { using type = char_tl_t<'l', 'u'>; };
		template <> struct type2fmt<unsigned long long> { using type = char_tl_t<'l', 'l', 'u'>; };

		template <> struct type2fmt<bool> { using type = char_tl_t<'d'>; };

		template <> struct type2fmt<float> { using type = char_tl_t<'f'>; };
		template <> struct type2fmt<double> { using type = char_tl_t<'l', 'f'>; };

		template <> struct type2fmt<nullptr_t> { using type = char_tl_t<'p'>; };

		// DBJ -- this makes char * output into 'p' aka pointer output
		// template <typename T> struct type2fmt<T*> { using type = char_tl_t<'p'>; };

		// DBJ -- BEGIN CHANGE
		template<class T, T v>
		struct my_integral_constant {
			static constexpr T value = v;
			typedef T value_type;
			typedef my_integral_constant type; // using injected-class-name
			constexpr operator value_type() const noexcept { return value; }
			constexpr value_type operator()() const noexcept { return value; } 
		};
        
		template< bool v_ >
		struct s_or_p : my_integral_constant<bool, v_ > {} ;

		template<> struct s_or_p<true> : my_integral_constant<bool, true >
		{
			using type = char_tl_t<'s'>;
		};

		template<> struct s_or_p<false> : my_integral_constant<bool, false >
		{
			using type = char_tl_t<'p'>;
		};

		template <
			typename T
		> 
		struct type2fmt<T*> 
		{
			using raw_T = remove_cv_t<T>;
			static constexpr bool is_str{ is_same<char,
					remove_cv_t<typename remove_ptr<raw_T>::type>>::value };

			using type = typename s_or_p<is_str>::type;
		};
		// DBJ -- END CHANGE

		template <typename T, typename FL>
		struct format_str {
			using raw_T = remove_cv_t<T>;
			static constexpr bool s_fmt{ contains<FL, char_t<'s'>>::value };

			static constexpr bool is_str{ is_same<char,
				remove_cv_t<typename remove_ptr<raw_T>::type>>::value };

			static constexpr bool is_int{ is_int_type<raw_T>::value };
			static constexpr bool has_x{ contains<FL, char_t<'x'>>::value };

			using raw_fmt = typename type2fmt<T>::type;

			using uint_x_fmt = typename conditional<is_int && has_x,
				substitute_t<
				substitute_t<raw_fmt, char_t<'d'>, char_t<'x'>>,
				char_t<'u'>, char_t<'x'>>,
				raw_fmt
				>::type;

			using fmt_type = typename conditional<s_fmt && is_str,
				substitute_t<raw_fmt, char_t<'p'>, char_t<'s'>>,
				uint_x_fmt
			>::type;

			using filtered_fl = remove_t<remove_t<FL, char_t<'x'>>, char_t<'s'>>;

			using type = append_t<filtered_fl, fmt_type>;
		};

		template <class InList, class OutList, size_t Counter>
		struct find_brace;

		template <class InList, class OutList>
		struct find_brace<tl<char_t<'}'>, InList>, OutList, 1> {
			using before = OutList;
			using after = InList;
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
			static_assert(N + 1 == N, "Missing } after {.");
		};

		template <typename SL, typename TL>
		struct autoformat;

		template <>
		struct autoformat<null_t, null_t> { using type = null_t; };

		template <typename TL>
		struct autoformat<null_t, TL> {
			using type = null_t;
			static_assert(always_false<TL>::value, "There are more vars than format tokens!");
		};

		template <typename SL, typename TL>
		struct autoformat<tl<char_t<'%'>, tl<char_t<'%'>, SL>>, TL>
		{
			using type = tl<char_t<'%'>, tl<char_t<'%'>, typename autoformat<SL, TL>::type>>;
		};

		template <typename SL, typename T, typename TL>
		struct autoformat<tl<char_t<'%'>, SL>, tl<T, TL>>
		{
			using type = tl<char_t<'%'>, typename autoformat<SL, TL>::type>;
		};

		template <typename SL, typename TL>
		struct autoformat<tl<char_t<'\\'>, tl<char_t<'{'>, SL>>, TL>
		{
			using type = tl<char_t<'{'>, typename autoformat<SL, TL>::type>;
		};

		template <typename SL, typename TL>
		struct autoformat<tl<char_t<'{'>, SL>, TL>
		{
			using other_brace = find_brace<SL, null_t, 1>;
			using format_block = typename other_brace::before;
			using rest_str = typename other_brace::after;

			static_assert(!is_same<TL, null_t>::value, "There are more {} than arguments to print");
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

		//#define AUTOFORMAT(s, ...) \
		//    ({ \
		//        struct strprov { static constexpr const char * str() { return static_cast<const char*>(s); } }; \
		//        using paramtypes = decltype(pprintpp::tie_types(__VA_ARGS__)); \
		//        using af = pprintpp::autoformat_t<strprov, paramtypes>; \
		//        af::str(); \
		//    })

		// DBJ -- replaced the above with bellow ... MSVC can work with this

#define AUTOFORMAT(FMT_, ...) \
    []() constexpr -> const char * { \
        using namespace jacek_galowicz; \
        struct strprov { static constexpr const char * str() { return static_cast<const char*>(FMT_); } }; \
        using paramtypes = decltype(pprintpp::tie_types(__VA_ARGS__)); \
        using af = pprintpp::autoformat_t<strprov, paramtypes>; \
        return af::str(); \
    }()

#ifndef pprintf
#define pprintf(s, ...) printf(AUTOFORMAT(s, ## __VA_ARGS__), ## __VA_ARGS__)
#else
#error macro pprintf already defined?
#endif

	} // pprintpp
} // namespace jacek_galowicz 

#endif // !_INC_PPRINTPP_
