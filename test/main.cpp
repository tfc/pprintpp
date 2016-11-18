#include <cassert>
#include <cstring>
#include <cstdio>
#include <pprintpp.hpp>

#define TEST(correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), correct_str))

int main()
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

    TEST("%c",   "{}", static_cast<char>(123));

    TEST("%d",   "{}", static_cast<short>(123));
    TEST("%d",   "{}", 123);
    TEST("%ld",  "{}", 123l);
    TEST("%lld", "{}", 123ll);

    TEST("%u",   "{}", 123u);
    TEST("%lu",  "{}", 123ul);
    TEST("%llu", "{}", 123ull);

    TEST("%x",   "{x}", 123u);
    TEST("%lx",  "{x}", 123ul);
    TEST("%llx", "{x}", 123ull);

    TEST("%f",  "{}", 1.0f);
    TEST("%lf", "{}", 1.0);

    TEST("%10d", "{10}", 123);
    TEST("%10x", "{10x}", 123u);
    TEST("%#10x", "{#10x}", 123u);

    puts("Green, green, green! All tests passed.\n");

    pprintf("{s} {} {1} {} {} {} {} {} {} {} {} {} {} {} {} {#x}\n",
            "1",2u,3.0,4.0f,5ull,'6',7ul,8,9,10,11,12,13,14,15,16u);

    return 0;
}
