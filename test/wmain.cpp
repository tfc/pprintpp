#define PPRINTPP_AVOID_STL
#define PPRINTPP_STANDARD_CHAR_PTR
#include "../include/pprintpp/pprintpp.hpp"

#include <cassert>
#include <cstring>
#include <typeinfo>
#include <cstdio>
#include <exception>

extern "C" int test_1(int = 0, wchar_t* [] = nullptr );
extern "C" int test_2(int = 0, wchar_t* [] = nullptr);
#ifdef PPRINTPP_STANDARD_CHAR_PTR
extern "C" int test_3(int = 0, wchar_t* [] = nullptr);
#endif

#define PPRINTPP_EXPERIMENTAL 1

#if PPRINTPP_EXPERIMENTAL
extern "C" int test_4(int = 0, wchar_t* [] = nullptr);
#endif

int wmain(int, wchar_t * [])
{
	printf("\nCompiled with: ");
#ifdef __clang__
	//__clang__             // set to 1 if compiler is clang
	//	__clang_major__       // integer: major marketing version number of clang
	//	__clang_minor__       // integer: minor marketing version number of clang
	//	__clang_patchlevel__  // integer: marketing patch level of clang
	//	__clang_version__     // string: full version number
	printf( "CLANG: %s\n" , __clang_version__);
#else
	printf("_MSVC_LANG: %lu\n", _MSVC_LANG);
#endif
	test_1();
	test_2();
	test_3();
#if PPRINTPP_EXPERIMENTAL
	test_4();
#endif
	printf("\nDone...\n");
	return EXIT_SUCCESS;
}
extern "C" int test_1(int, wchar_t* [])
{
	// DBJ  -- added 
#ifdef _MSC_VER
	// NOTE: 
	// on windows10 make sure you set the console font that can display your glyphs
	// Lucida Console seems ok for cyrillic glyphs
	pprintf("\n\n{s}\n\n", u8"Душан Јовановић (u8 string literal)" ); // utf8 encoded char
#endif // _MSC_VER

	pprintf("This is \\{ how you mask } {s}.\n", "curly braces");

	pprintf("Of course it's possible to add more formatting flags:\n"
		"{x}, {10x}, {#10x}, and {#010x} use the normal printf capabilities.\n",
		0x123, 0x123, 0x123, 0x123);

	pprintf("{20}\n", 1.23456);

#ifdef __clang__
	// this is apparently too complex for: 
	// Microsoft (R) C/C++ Optimizing Compiler Version 19.23.28106.4 for x86
	// pprintpp.hpp(156, 1) : fatal error C1202 : recursive type or function dependency context too complex
	// 
	pprintf("{35s} | Format str -> Result\n"
		"{35s} | ---------------------\n"
		"{35s} | \\{s}     -> \"{s}\"\n"
		"{35s} | \\{10s}   -> \"{10s}\"\n"
		"{35s} | \\{}      -> \"{}\"\n"
		"{35s} | \\{x}     -> \"{x}\"\n"
		"{35s} | \\{10}    -> \"{10}\"\n"
		"{35s} | \\{10x}   -> \"{10x}\"\n"
		"{35s} | \\{#10x}  -> \"{#10x}\"\n"
		"{35s} | \\{#010x} -> \"{#010x}\"\n"
		"{35s} | \\{}      -> \"{}\"\n"
		"{35s} | \\{10}    -> \"{10}\"\n"
		"{35s} | \\{5.2}   -> \"{5.2}\"\n",
		"Meaning", "---------------------",
		"String \"abc\"", "abc",
		"String \"abc\" + min width", "abc",
		"value 0x123, default", 0x123,
		"value 0x123, hex", 0x123,
		"minimum width", 0x123,
		"hex + min width", 0x123,
		"hex + min width + hex prefix", 0x123,
		"hex + min w. + hex prefix + 0-pad", 0x123,
		"FP", 12.345,
		"FP + min width", 12.34567890123456789,
		"FP + width + max precision", 12.34567890123456789
	);
#endif // __clang__
	return EXIT_SUCCESS;
}
/*
Example:
 auto compile_time_format_ =  AUTOFORMAT("{} {}", 123, 1.23f)
// must return: "%d %f"
 */
#define PPRINTPP_TEST(printf_format_correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), printf_format_correct_str))

extern "C" int test_2(int, wchar_t* [])
{
	PPRINTPP_TEST("", "");

	PPRINTPP_TEST("%%", "%%");
	PPRINTPP_TEST("%d %f", "{} %f", 123, 1.23f);
	PPRINTPP_TEST("%f %d", "%f {}", 1.23f, 123);

	PPRINTPP_TEST(" { ", " \\{ ");
	PPRINTPP_TEST("{}", "\\{}");
	PPRINTPP_TEST(" { %d } ", " \\{ {} } ", 123);

	PPRINTPP_TEST("%p", "{}", nullptr);
	PPRINTPP_TEST("%p", "{}", reinterpret_cast<void*>(0));


#ifdef PPRINTPP_STANDARD_CHAR_PTR
	PPRINTPP_TEST("%p", "{}", "str");
#else
	PPRINTPP_TEST("%s", "{}", "str");
#endif

	PPRINTPP_TEST("%s", "{s}", "str");

	PPRINTPP_TEST("%c", "{}", static_cast<char>(123));

	PPRINTPP_TEST("%d", "{}", static_cast<short>(123));
	PPRINTPP_TEST("%d", "{}", 123);
	PPRINTPP_TEST("%ld", "{}", 123l);
	PPRINTPP_TEST("%lld", "{}", 123ll);

	PPRINTPP_TEST("%u", "{}", 123u);
	PPRINTPP_TEST("%lu", "{}", 123ul);
	PPRINTPP_TEST("%llu", "{}", 123ull);

	PPRINTPP_TEST("%x", "{x}", 123u);
	PPRINTPP_TEST("%lx", "{x}", 123ul);
	PPRINTPP_TEST("%llx", "{x}", 123ull);

	PPRINTPP_TEST("%d", "{}", true);

	PPRINTPP_TEST("%f", "{}", 1.0f);
	PPRINTPP_TEST("%lf", "{}", 1.0);

	PPRINTPP_TEST("%10d", "{10}", 123);
	PPRINTPP_TEST("%10x", "{10x}", 123u);
	PPRINTPP_TEST("%#10x", "{#10x}", 123u);

	// Give the user hex if asked for explicitly.
	PPRINTPP_TEST("%x", "{x}", 123);
	PPRINTPP_TEST("%lx", "{x}", 123l);
	PPRINTPP_TEST("%llx", "{x}", 123ll);

	pprintf( "\n\n[" __TIMESTAMP__ "] {s}",  "All basic tests have passed.");

	return 0;
}
#undef PPRINTPP_TEST

#ifdef PPRINTPP_STANDARD_CHAR_PTR
extern "C" int test_3(int, wchar_t* [])
{
	std::exception x1("runtime      error");
	std::exception x2("cosmological error");

	// DBJ -- {s} is not required for string output
	pprintf("\nException: { } \nException: { }\n", x1.what(), x2.what() );

	const char* slit = "STRING LITERAL";
	pprintf("\nString literal without 's': { } ", slit );

	[[maybe_unused]] auto dumzy = true;
	return EXIT_SUCCESS;
}
#endif // PPRINTPP_STANDARD_CHAR_PTR
