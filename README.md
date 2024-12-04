## dtl-modern

The `dtl-modern` library is the modern Diff Template Library written in C++20 as an improvement of the [`dtl`](https://github.com/cubicdaiya/dtl) library written by [`cubicdaiya`](https://github.com/cubicdaiya).

## Table of contents

- [Features](#features)
- [Getting started](#getting-started)
  - [Comparing two strings](#comparing-two-strings)
  - [Comparing two sequences that have arbitrary type](#comparing-two-sequences-that-have-arbitrary-type)
  - [Difference as Unified Format](#difference-as-unified-format)
  - [Comparing two large sequences](#comparing-two-large-sequences)
  - [Calculate edit distance only](#calculate-only-edit-distance)
  - [Displaying diff](#displaying-diff)
  - [Unserious difference](#unserious-difference)
  - [Merge three sequences](#merge-three-sequences)
  - [Patch function](#patch-function)
- [Algorithm](#algorithm)
  - [Computational complexity](#computational-complexity)
  - [Comparison when difference between two sequences is very large](#comparison-when-difference-between-two-sequences-is-very-large)
  - [Implementations with various programming languages](#implementations-with-various-programming-languages)
- [Examples](#examples)
- [Tests](#tests)
- [License](#license)

## Features

The `dtl-modern` library provides functions for comparing two sequences/ranges that have arbitrary type.

The type must be comparable i.e. support equality comparison `a == b`.

> - TODO: relax this requirement by adding support for explicit comparison function at call site

The sequences must support **`std::random_access_iterator`** / **`std::ranges::random_access_range`**.

> You can see the full requirement in [common.hpp](include/dtl_modern/common.hpp)

## Getting started

To start using this library, all you need to do is include `dtl.hpp`.

```cpp
#include <dtl_modern/dtl_modern.hpp>
```

### Comparing two strings

```cpp
#include <dtl_modern/dtl_modern.hpp>

#include <string>
#include <string_view>

int main()
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    auto hello1 = "hello World!"sv;    // notice the different type, this one is std::string_view
    auto hello2 = "Hell word"s;        // this one is std::string

    // NOTE: passing const char[N] will work, but const char* won't (wrap it in std::string_view)

    // as long as the two ranges have the same element type, you can compare them directly
    auto [lcs, ses, edit_distance] = dtl_modern::diff(hello1, hello2);

    // ...
}
```

> You can read the source file to see the difference in usage between `dtl` and `dtl-modern`: [strdiff.cpp](examples/source/strdiff.cpp).

When the above code is run, `dtl-modern` calculates the difference between A and B as **Edit Distance**, **LCS**, and **SES**.

| Term          | Description                                                                                                               |
| ------------- | ------------------------------------------------------------------------------------------------------------------------- |
| Edit Distance | Edit Distance is numerical value for declaring a difference between two sequences.                                        |
| LCS           | LCS stands for Longest Common Subsequence.                                                                                |
| SES           | SES, stands for Shortest Edit Script, is the shortest course of action for tranlating one sequence into another sequence. |

If one sequence is `"abc"` and another sequence is `"abd"`, the **Edit Distance**, **LCS**, and **SES** are below.

| Metric        | Value             |
| :------------ | :---------------- |
| Edit Distance | `2`               |
| LCS           | `ab`              |
| SES           | `C a C b D c A d` |

With `C` stands for `Common`, `D` stands for `Delete`, and `A` stands for `Add`.

### Comparing two sequences that have arbitrary type

`dtl-modern` can compare two sequences with arbitrary data as long as both of the sequences contain the same type.

For example, `dtl-modern` can compare two `int` ranges like the example below.

```cpp
#include <dtl_modern/dtl_modern.hpp>

#include <array>
#include <vector>

int main()
{
    auto a = std::array { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto b = std::vector{ 3, 5, 1, 4, 5, 1, 7, 9, 6, 10 };

    auto [lcs, ses, edit_distance] = dtl_modern::diff(a, b);

    // ...
}
```

> You can read the source file to see the difference in usage between `dtl` and `dtl-modern`: [intdiff.cpp](examples/source/intdiff.cpp).

### Difference as Unified Format

You can generate [Unified Format](http://www.gnu.org/s/diffutils/manual/html_node/Unified-Format.html) using `dtl-modern`.

```cpp
#include <dtl_modern/dtl_modern.hpp>

int main()
{
    using namespace std::string_view_literals;

    auto a = "acbdeaqqqqqqqcbed"sv;   // const char* didn't work for diff and unidiff
    auto b = "acebdabbqqqqqqqabed"sv;

    // directly generate unified format hunks, lcs, ses, and edit distance
    auto [uni_hunks, lcs, ses, edit_distance] = dtl_modern::unidiff(a, b);

    // ...

    // or generate unified format from ses generated beforehand
    auto uni_hunks = dtl_modern::ses_to_unidiff(ses);

    // ...
}
```

> Previous example of [intdiff.cpp](examples/source/intdiff.cpp) used `unidiff`, you can read it directly.
>
> - TODO : implement function that translates one sequence into another sequence with Unified Format.

### Comparing two large sequences

When Comparing two large sequences, you can enable the `huge` flag on `dtl_modern::diff` and `dtl_modern::unidiff` function (default = `false`).

> What the flag does is reserve a memory of size `2^21 x sizeof(Element)` at the start of diff-ing.

```cpp
#include <dtl_modern/dtl_modern.hpp>

int main ()
{
    // ...

    {
        // set the huge flag to true, the default value is false
        auto [lcs, ses, edit_distance] = dtl_modern::diff(a, b, true);

        // ...
    }

    {
        // set the huge flag to true, the default value is false
        auto [uni_hunks, lcs, ses, edit_distance] = dtl_modern::unidiff(a, b, true);

        // ...
    }

    // ...
}
```

### Calculate edit distance only

If you need edit distance only, you can use this function since the calculation of edit distance is lighter than the calculation of LCS and SES.

```cpp
#include <dtl_modern/dtl_modern.hpp>

#include <array>
#include <vector>

int main()
{
    auto a = std::array { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto b = std::vector{ 3, 5, 1, 4, 5, 1, 7, 9, 6, 10 };

    auto edit_distance = dtl_modern::edit_distance(a, b);

    // ...
}
```

### Displaying diff

Unlike `dtl`, `dtl-modern` consider this feature to be "optional" thus separating this feature into its own directory, [extra](include/dtl_modern/extra).

No additional flag is needed to use this feature, you just need to `#include` the required header to use it.

```cpp
// for displaying SES
#include <dtl_modern/extra/ses_display_simple.hpp>

// for displaying Unified Format hunks
#include <dtl_modern/extra/uni_hunk_display_simple.hpp>
```

To display the SES/Unified Format hunks, you need to call `dtl_modern::extra::display` function. This function will return a type that wraps the argument into a type that can be displayed using `std::format` (or `fmt::format`; instruction below ) and/or `std::ostream&` using the `operator<<` overload.

The display functionality relies on `std::formatter` specialization for `std::format`. If you are using `fmt` library instead of the standard library, you can define macro `DTL_MODERN_DISPLAY_FMTLIB` to use `fmt::formatter` implementation instead. This action will replace the `std::formatter` specialization thus disabling `std::format` functionality and vice versa. There is plan to add an option to enable both.

> NOTE: the `fmt` library itself is not provided by this library. To enable `fmt` feature you need to have `fmt` available and linked to your project where this library is used.

```cpp
#include <dtl_modern/dtl_modern.hpp>

// uncomment this define to use the standard library <format> instead
#define DTL_MODERN_DISPLAY_FMTLIB
#include <dtl_modern/extra/ses_display_simple.hpp>
#include <dtl_modern/extra/uni_hunk_display_simple.hpp>

// using fmt
#include <fmt/core.h>

#include <array>
#include <vector>

int main()
{
    auto a = std::array { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto b = std::vector{ 3, 5, 1, 4, 5, 1, 7, 9, 6, 10 };

    auto [uni_hunks, lcs, ses, edit_distance] = dtl_modern::unidiff(a, b);

    fmt::println("\nSES:\n");
    fmt::println("{}", dtl_modern::extra::display(ses));

    fmt::println("\nUnified Format:\n");
    fmt::println("{}", dtl_modern::extra::display(uni_hunks));
}
```

### Unserious difference

> - NOTE: not implemented yet for `dtl-modern`

### Merge three sequences

> - NOTE: not implemented yet for `dtl-modern`

### Patch function

> - NOTE: not implemented yet for `dtl-modern`

## Algorithm

The algorithm `dtl` (in turns `dtl-modern`) uses is based on [An O(NP) Sequence Comparison Algorithm](<https://doi.org/10.1016/0020-0190(90)90035-V>) as described by Sun Wu, Udi Manber and Gene Myers. This algorithm is an efficient algorithm for comparing two sequences.

### Computational complexity

The computational complexity of Wu's O(NP) Algorithm is averagely O(N+PD), in the worst case, is O(NP).

### Comparison when difference between two sequences is very large

Calculating LCS and SES efficiently at any time is a difficult and the calculation of LCS and SES needs massive amount of memory when a difference between two sequences is very large.

The `dtl` (in turns `dtl-modern`) avoids the above problem by dividing each sequence into plural sub-sequences and joining the difference of each sub-sequence at the end.

> This feature is supported after version 0.04

### Implementations with various programming languages

There are the Wu's O(NP) Algorithm implementations with various programming languages below.

https://github.com/cubicdaiya/onp

## Examples

There are examples in the [examples](examples) directory. I used Conan to be able to use external libraries for the examples. Make sure it is installed and configured in your system.

To build the examples, you first must change your current working directory to `examples`, then run these commands

```sh
conan install . --build missing -s build_type=Debug   # or Release
cmake --preset conan-debug                            # conan-release if Release mode, conan-default if you're on windows regardless
cmake --build --preset conan-debug                    # adjust the preset like above
```

## Tests

Just like examples, the tests requires Conan to be able to be built and ran.

Make sure you are in the `test` directory first, then run these commands to build the tests

```sh
conan install . --build missing -s build_type=Debug   # or Release
cmake --preset conan-debug                            # conan-release if Release mode, conan-default if you're on windows regardless
cmake --build --preset conan-debug                    # adjust the preset like above
```

To run the tests itself, you can run each binary directly or use `ctest`

```sh
ctest --preset conan-debug --output-on-failure        # adjust the preset like above
```

## License

> - `dtl`: [COPYING](https://github.com/cubicdaiya/dtl/blob/master/COPYING)
> - `dtl-modern` [COPYING](COPYING)
