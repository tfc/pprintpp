# `pprintpp` Documentation

## How to use

It's easy. Just add the library to your include path, and include `<pprintpp.hpp>`.
`pprintpp.hpp` defined the macro `pprintf`, which just depends on the original `printf`.

``` c++
#include <cstdio>
#include <pprintpp.hpp>

int main()
{
  pprintf("Hello {s}!, {} {} {}\n", "world", 123, 123ull, 1.23);
}
```

## "Advanced" Usage

`pprintpp` tries to be as `printf` compatible as possible, but there are some pecularities:

### Adding Additional Format Directives

`printf` historically supports indendation, hex printing of unsigneds, etc. via additional 
*optional* format specifiers between `%` and the actual type format character.

`pprintpp` accepts these between the `{}` brackets. Examples:

``` c++
pprintf("Int with 10 characters indentation: {10}\n", 123);
pprintf("Unsigned long long or whatever in hex notation: {#8x}\n", static_cast<unsigned long long>(0x123));
```

### Printing Actual `{` Braces

Braces can be normally printed as before `pprintpp`, but the opening ones have to be masked in order to 
tell `pprintpp` to not interprete these as place holders:

``` c++
pprintf("These are braces: \\{}.\n");
pprintf("JSON document: \\{ "foo" : "bar", "value" : {} }\n", 123);
```
