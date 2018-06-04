#include <pprintpp.hpp>

#include <cstdio>

int main()
{
    pprintf("{} hello {s}! {}\n", 1, "world", 2.0) ;

    pprintf("This is \\{ how you mask } {s}.\n", "curly braces");

    pprintf("Of course it's possible to add more formatting flags:\n"
            "{x}, {10x}, {#10x}, and {#010x} use the normal printf capabilities.\n",
            0x123, 0x123, 0x123, 0x123);

    pprintf("{20}\n", 1.23456);

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
}
