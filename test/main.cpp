#include <cassert>
#include <cstring>
#include <pprintpp.hpp>

#define TEST(correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), correct_str))

int main()
{
    TEST("", "");

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

    TEST("%f",  "{}", 1.0f);
    TEST("%lf", "{}", 1.0);

    pprintf("Green, green, green! All tests passed.\n");

    return 0;
}
