
#include "../pprintpp.hpp"

#include <typeinfo>
#include <cstdio>

extern "C" int test(int, char* []);

#ifdef _MSC_VER
int wmain(int, wchar_t * argv[])
#else
int main(int, char * argv[])
#endif // _MSC_VER
{
	// DBJ  -- added 
#ifdef _MSC_VER
	// NOTE: make sure you set the font that can display your glyphs
	// Lucida Console seems ok for cyrillic glyphs
	// no can do -- pprintf("{S}\n\n", L"Dušan Jovanović" );
	pprintf("\n\n{s}\n\n", u8"Dusan Jovanovic in cyrillic: Душан Јовановић" ); // utf8 encoded char
	// prints pointer -- pprintf("{}\n\n", u8"Душан Јовановић" ); // utf8 encoded char
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
    // standard tests
	test(0, 0);

}

#include "../pprintpp.hpp"

#include <cassert>
#include <cstring>
#include <cstdio>

/*
Example:
 auto compile_time_format_ =  AUTOFORMAT("{} {}", 123, 1.23f)
// must return: "%d %f"
 */
#define TEST(printf_format_correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), printf_format_correct_str))

extern "C" int test(int, char* [])
{
	TEST("", "");

	TEST("%%", "%%");
	TEST("%d %f", "{} %f", 123, 1.23f);
	TEST("%f %d", "%f {}", 1.23f, 123);

	TEST(" { ", " \\{ ");
	TEST("{}", "\\{}");
	TEST(" { %d } ", " \\{ {} } ", 123);

	TEST("%p", "{}", nullptr);
	TEST("%p", "{}", reinterpret_cast<void*>(0));

	// For safety reasons:
	// Only print strings as strings, if the user also writes {s}
	TEST("%p", "{}", "str");
	TEST("%s", "{s}", "str");

	TEST("%c", "{}", static_cast<char>(123));

	TEST("%d", "{}", static_cast<short>(123));
	TEST("%d", "{}", 123);
	TEST("%ld", "{}", 123l);
	TEST("%lld", "{}", 123ll);

	TEST("%u", "{}", 123u);
	TEST("%lu", "{}", 123ul);
	TEST("%llu", "{}", 123ull);

	TEST("%x", "{x}", 123u);
	TEST("%lx", "{x}", 123ul);
	TEST("%llx", "{x}", 123ull);

	TEST("%d", "{}", true);

	TEST("%f", "{}", 1.0f);
	TEST("%lf", "{}", 1.0);

	TEST("%10d", "{10}", 123);
	TEST("%10x", "{10x}", 123u);
	TEST("%#10x", "{#10x}", 123u);

	// Give the user hex if asked for explicitly.
	TEST("%x", "{x}", 123);
	TEST("%lx", "{x}", 123l);
	TEST("%llx", "{x}", 123ll);

	puts("Green, green, green! All tests passed.\n");

	pprintf("{s} {} {1} {} {} {} {} {} {} {} {} {} {} {} {} {#x}\n",
		"1", 2u, 3.0, 4.0f, 5ull, '6', 7ul, 8, 9, 10, 11, 12, 13, 14, 15, 16u);

	return 0;
}

