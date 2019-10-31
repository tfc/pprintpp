#define PPRINTPP_AVOID_STL
#define PPRINTPP_STANDARD_CHAR_PTR
#include "../../include/pprintpp/pprintpp.hpp"
#include <cassert>
#include <vector>
#include <cstdio>
#include <memory>
#include <tuple>

using namespace std;
/*
every variadic argument is argument to this function
unless user has not specilazied an overload for user defined type

this returns constexpr T, which is a `tall order`
might be speeding things up, but the hard requirement 
is that argument type must be a "literal type"
*/
template <typename T>
inline constexpr T ppf_arg(T const & value) noexcept
{
	return value;
}

//
//
template<typename ... Args>
inline auto	to_buff (char const* fmt, Args ...args) noexcept -> unique_ptr<char[]>
{
	assert(fmt != nullptr);
	/*
     I do now want to call the sequence of frm_args() twice

	 first
	size_t size = 1 + std::snprintf(nullptr, 0, fmt, ppf_arg(args) ...);

	 and then
	size = std::snprintf(buf.get(), size, fmt, ppf_arg(args) ...);

	ppf_arg's are called only once 
	rezults are stored in a tuple
	*/
	auto args_tup = make_tuple( ppf_arg(args) ...);
	auto snprintf_tup_1 = make_tuple(nullptr, 0, fmt );

	// obtain the buffer size required
	size_t size = apply(std::snprintf, tuple_cat(snprintf_tup_1, args_tup));
	assert(size > 0);

	// unique_ptr<char[]> is the fastest standard C++, runtime char buffer
	auto buf = std::make_unique<char[]>(size + 1);
	auto snprintf_tup_2 = make_tuple(buf.get(), size, fmt);
	// populate the buffer, do not call the  ppf_arg's again
	size = apply(std::snprintf, tuple_cat(snprintf_tup_2, args_tup));
	assert(size > 0);
	// move out the unique_ptr
	return buf;
}
///////////////////////////////////////////////////////////////////////
// user defined type 'Foo'
struct Foo {
	const char* name = "Foo";
};

// 'Foo' argument processor for to_buff
// this is not a good design, it couples 'Foo' and the 'ppf_arg()' and 'to_buff()' mechanism
// but some users, really want this ...
// and by the way, one can adorn this with constexpr retval
// but first requirement is that argument is a "literal type"
// and the 'Foo' is
inline constexpr char const * ppf_arg(Foo const & foo_ ) noexcept
{
	return foo_.name;
}

// std:: containers
inline char const* ppf_arg(vector<char> const & v_) noexcept
{
	// CAUTION! last char might not be a string terminator aka '\0'
	assert( v_.back() == char(0) );
	return v_.data();
}

///////////////////////////////////////////////////////////////////////
extern "C" int test_4(int, wchar_t* [])
{
	/*
	this compiles but will likely crash the program as printf() input is wrong
	if good compiler will understand what is required and will replace them Foo's
	with "A" and "B"
	*/
	pprintf("\n\nFoo: %s, Foo: %s, %s %d", Foo{ "A" }, Foo{ "B" }, "The answer is:", 42);

	/*
	this does not compile , there is no transformation of 'Foo' instances to the 'char *'

	pprintf("\n\nFoo: {s}, Foo: {s}, {s} {d}\n\n", Foo{ "A" }, Foo{ "B" }, "The answer is:", 42);
	*/

	/*	this compiles and works and is snazzy C++, alas completely unrelated to pprintf.	
	    internaly one ppf_arg() is called for each argument, allways.
		regardless of the arguments type
	*/
	{
		auto rez{ to_buff(
			"\n\nFoo: %s, Foo: %s, %s %d\n\n", Foo{"A"}, Foo{"B"}, "The answer is:", 42
		  ) };
				pprintf("{s}", rez.get());
	}

	/*
	pprintf and user defined types

	now the following will compile and work and will use pprintf
	Foo depends on ppf_arg() overload to be also user defined, it is coupled to it
	*/
	pprintf("\n\nFoo: {s}, Foo: {s}, {s} {d}", ppf_arg(Foo{ "A" }), ppf_arg(Foo{ "B" }), "The answer is:", 42);

	// using std:: container
	vector<char> vb{'V','e' ,'c' ,'t' ,'o','r', char(0) };

	pprintf("\n\nVector of chars contains: \"{s}\"", ppf_arg(vb));

	return EXIT_SUCCESS;
}
