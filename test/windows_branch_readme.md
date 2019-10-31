
### Notes on windows branch

- WINDOWS branch is built using Visual Studio 2019 16.3.7, with both CL and CLANG compiler (8.0.1)
as delivered with it.
   - As of today (2019-10-31) VS2019 C++ compiler is:
Microsoft (R) C/C++ Optimizing Compiler Version 19.23.28106.4 for x86
- Visual Studio 2019 is being updated very often.
   - Please check, from time to time, if CL capabilities are 
improved so it can compile the code that CLANG could, but CL could not.
   - See the `test_1()`.


#### pprintf provides additional safety to its users

It treats `char *` as a pointer, not as a string.

In case users require the ::printf behaviour,  before including the pprintf, they can `#define PPRINTPP_STANDARD_CHAR_PTR 1` to have '::printf()` standard behaviour

```cpp
#define PPRINTPP_STANDARD_CHAR_PTR 1
#include <pprintpp.h>

char const * message_ = "The Message";

#if PPRINTPP_STANDARD_CHAR_PTR 
    // standard
    // output the string
    pprintf("{}", message_) ;
    // output the pointer
    pprintf("{p}", message_) ;
#else
    // safe
    // output the string
    pprintf("{s}", message_) ;
    // output the pointer
    pprintf("{}", message_) ;
#endif
```

#### Experimental

Currently the experimental subfolder contains `ppf_arg`. Schema that might be used to somewhat serve the users wanting to handle their user defined types.

The purpose of the experimental stuff. is to be used for discussions. Will be eihter incorporated or erased.

#### The roadmap

1. testing [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer)
1. support for `wchar_t`
   - Basically implementing `wpprintf` that will target [`::wprintf`](https://en.cppreference.com/w/c/io/fwprintf)
   - All Windows API functions that accept LP[C]WSTR (that is: are wchar_t based), expect UTF-16 encoding.
   - The size of wchar_t is implementation-defined, and in fact most Linux compilers (such as GCC) define it to be 32-bit large, designed to hold text in UTF-32 encoding.
   - With MSVC compiler, `wchar_t` is 16-bit large, with the intention that it be used to represent Unicode strings in UTF-16 encoding.
   - Windows native `char` is `wchar_t`
      - All Windows 'char' stdio is transformed to `wchar_t` before final output
      - Windows 'char' stdio is performance overhead

```cpp
wpprintf(L"{s}{d}{c}", L"The answer is: ", 42, L'!' );
```

- there are standard formaters in `wprintf` to output narrow strings and chars, and vice versa one can output wide strings and chars using `printf`.
   - that incures performance overhead
