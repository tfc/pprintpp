#include <pprintpp/pprintpp.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

#define TEST(correct_str, ...)                                                 \
  assert(!strcmp(AUTOFORMAT(__VA_ARGS__), correct_str))

int main() {
  TEST("", "");

  TEST("%%", "%%");
  TEST("%d %f", "{} %f", 123, 1.23f);
  TEST("%f %d", "%f {}", 1.23f, 123);

  TEST(" { ", " \\{ ");
  TEST("{}", "\\{}");
  TEST(" { %d } ", " \\{ {} } ", 123);

  TEST("%p", "{}", nullptr);
  TEST("%p", "{}", reinterpret_cast<void *>(0));

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

  TEST("%X", "{X}", 123u);
  TEST("%lX", "{X}", 123ul);
  TEST("%llX", "{X}", 123ull);

  TEST("%b", "{b}", 123u);
  TEST("%lb", "{b}", 123ul);
  TEST("%llb", "{b}", 123ull);

  TEST("%B", "{B}", 123u);
  TEST("%lB", "{B}", 123ul);
  TEST("%llB", "{B}", 123ull);

  TEST("%o", "{o}", 123u);
  TEST("%lo", "{o}", 123ul);
  TEST("%llo", "{o}", 123ull);

  TEST("%O", "{O}", 123u);
  TEST("%lO", "{O}", 123ul);
  TEST("%llO", "{O}", 123ull);

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

  pprintf("{s} {} {1} {} {} {} {} {} {} {} {} {} {} {} {} {#x}\n", "1", 2u, 3.0,
          4.0f, 5ull, '6', 7ul, 8, 9, 10, 11, 12, 13, 14, 15, 16u);

  return 0;
}
