String Formatting
=================

Corelib does not use the C standard library's format specifiers.

Format specifiers follow this prototype:

<tt>{specifier,arguments}</tt>

Example:

```cpp
println( "'{u32,xu,4}'", 10 );
```

Output:

```sh
' 0xA'
```

Arguments are case-insensitive.

Data is always assumed to be passed in by value, unless otherwise stated.

When an argument is invalid, format specifier is printed.

When two arguments are conflicting, the last argument takes precedence.

@note Arguments written in italics describe
a string literal, not the name of an argument.
Example: <i>+int</i> means a positive integer.

Escape Braces
-------------

```
{{
```

### Description

Write a literal open curly brace.

### Arguments

This specifier takes no arguments.

Boolean
-------

```
{b}
```

### Description

Write a <tt>#b32</tt>.

@note Value is read in as an <tt>int</tt>. <tt>b8</tt>, <tt>b16</tt> and <tt>b32</tt> get promoted to <tt>int</tt>. <tt>b64</tt> and <tt>bsize</tt> should be cast to <tt>int</tt>.

Non-zero is interpreted as ```true```.

Zero is interpreted as ```false```.

### Arguments

| Argument                         | Description                                                                                                                      |
| -------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| <tt>\*<i>+int, optional</i></tt> | Indicates that value is passed in as a pointer. If integer is provided, specifies how many booleans to print from pointer.       |
| <tt>*_</tt>                      | Indicates that value is passed in as a pointer. Number of booleans to print is passed in before pointer. Max number is #I32_MAX. |
| <tt><i>+int</i></tt>             | Left side padding.                                                                                                               |
| <tt><i>-int</i></tt>             | Right side padding.                                                                                                              |
| <tt>b</tt>                       | Write boolean as binary <tt>1</tt> or <tt>0</tt> instead of <tt>true</tt> or <tt>false</tt>.                                     |

UTF-8 Character
---------------

```
{c}
```

### Description

Write a <tt>char</tt>.

@note
- When passed by value, value is read in as an <tt>int</tt>. <tt>char</tt> gets promoted to <tt>int</tt>.
- When read by pointer, value is read in as a <tt>char*</tt>.

### Arguments

| Argument                         | Description                                                                                                                            |
| -------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| <tt>\*<i>+int, optional</i></tt> | Indicates that value is passed in as a pointer. If integer is provided, specifies how many characters to print from pointer.           |
| <tt>\*_</tt>                     | Indicates that value is passed in as a pointer. Number of characters to print is passed in before pointer. Max number is #I32_MAX.     |
| <tt>r<i>+int, optional</i></tt>  | Repeat character. If no integer is provided, prints character twice.                                                                   |
| <tt>r_</tt>                      | Repeat character. Number of times to print is provided as an argument before character. Max number is #I32_MAX. Cannot be used with *_ |
| <tt><i>+int</i></tt>             | Left side padding.                                                                                                                     |
| <tt><i>-int</i></tt>             | Right side padding.                                                                                                                    |
| <tt>u</tt>                       | Character is printed as upper-case if it's an alphabetic character.                                                                    |
| <tt>l</tt>                       | Character is printed as lower-case if it's an alphabetic character.                                                                    |

Null Terminated UTF-8 String
----------------------------

```
{cc}
```

### Description

Write a null terminated string.

@note Value is read in as a <tt>const char\*</tt>.

### Arguments

| Argument               | Description                                                                                            |
| ---------------------- | ------------------------------------------------------------------------------------------------------ |
| <tt>\*<i>+int</i></tt> | Indicate maximum number of characters to print. If max is > string length, prints up to string length. |
| <tt>\*_</tt>           | Indicates that maximum number of characters to print is passed in before string. Max is #I32_MAX       |
| <tt><i>+int</i></tt>   | Left side padding.                                                                                     |
| <tt><i>-int</i></tt>   | Right side padding.                                                                                    |
| <tt>u</tt>             | Characters are printed as upper-case.                                                                  |
| <tt>l</tt>             | Characters are printed as lower-case.                                                                  |

Floating Point Number
---------------------

```
{f} {f32} {f64}
```

### Description

Write an <tt>f64</tt>.

@note
- When passed by value, value is read in as an <tt>f64</tt>. <tt>f32</tt> gets promoted to <tt>f64</tt>.
- When passed by pointer, value is read in as a pointer to specified type. <tt>{f}</tt> always assumes <tt>f32</tt>.

### Arguments

| Argument                                      | Description                                                                                                              |
| --------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| <tt>\*<i>+int, optional</i></tt>              | Indicates that value is passed in as a pointer. If integer is provided, specifies how many floats to print from pointer. |
| <tt>\*_</tt>                                  | Indicates that value is passed in as a pointer. Number of floats to print is passed in before float. Max is #I32_MAX     |
| <tt><i>+int</i></tt>                          | Left side padding with spaces.                                                                                           |
| <tt><i>-int</i></tt>                          | Right side padding with spaces.                                                                                          |
| <tt>0<i>+int</i></tt>                         | Left side padding with zeroes.                                                                                           |
| <tt><i>padding, optional</i>.<i>+int</i></tt> | Precision to print fractional part at. Default = 6.                                                                      |
| <tt>s</tt>                                    | Separate every 3 whole number digits by a comma.                                                                         |
| <tt>m</tt>                                    | Assume number represents bytes. Formats as bytes, kilobytes, megabytes or terabytes as appropriate.                      |
| <tt>mib</tt>                                  | Assume number represents bytes. Formats as bytes, kibibytes, mebibytes or tebibytes as appropriate.                      |

Vector
------

```
{v2} {v3} {v4}
```

### Description

Write a <tt>#Vector2</tt>, <tt>#Vector3</tt> or <tt>#Vector4</tt>.

@note Value is read as specified type.

### Arguments

| Argument                                      | Description                                                                                                              |
| --------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| <tt>\*<i>+int, optional</i></tt>              | Indicates that value is passed in as a pointer. If integer is provided, specifies how many floats to print from pointer. |
| <tt>\*_</tt>                                  | Indicates that value is passed in as a pointer. Number of floats to print is passed in before float. Max is #I32_MAX     |
| <tt><i>+int</i></tt>                          | Left side padding with spaces.                                                                                           |
| <tt><i>-int</i></tt>                          | Right side padding with spaces.                                                                                          |
| <tt>0<i>+int</i></tt>                         | Left side padding with zeroes.                                                                                           |
| <tt><i>padding, optional</i>.<i>+int</i></tt> | Precision to print fractional part at. Default = 6.                                                                      |
| <tt>s</tt>                                    | Separate every 3 whole number digits by a comma.                                                                         |

Integer
-------

```
{i} {i8} {i16} {i32} {i64} {isize}
{u} {u8} {u16} {u32} {u64} {usize}
```

### Description

Write an integer.

@note
Value is read in as <tt>int</tt> if bit-depth is < 32.
Otherwise read in as specified type.
<tt>{i}</tt> and <tt>{u}</tt> are assumed to be 32-bit.

### Arguments

| Argument                          | Description                                                                                                                |
| --------------------------------- | -------------------------------------------------------------------------------------------------------------------------- |
| <tt>\*<i>+int, optional</i></tt>  | Indicates that value is passed in as a pointer. If integer is provided, specifies how many integers to print from pointer. |
| <tt>\*_</tt>                      | Indicates that value is passed in as a pointer. Number of integers to print is passed in before integer. Max is #I32_MAX   |
| <tt><i>+int</i></tt>              | Left side padding with spaces.                                                                                             |
| <tt><i>-int</i></tt>              | Right side padding with spaces.                                                                                            |
| <tt>0<i>+int</i></tt>             | Left side padding with zeroes.                                                                                             |
| <tt>s</tt>                        | Write comma every 3 digits for decimal, tick every 2 digits for hex and every 8 digits for binary.                         |
| <tt>xu</tt>                       | Write as upper-case hexadecimal.                                                                                           |
| <tt>xl</tt>                       | Write as lower-case hexadecimal.                                                                                           |
| <tt>b</tt>                        | Write as binary.                                                                                                           |
| <tt>f</tt>                        | Write up to full integer width.                                                                                            |

UTF-8 String
------------

```
{s}
```

### Description

Write a <tt>#String</tt>.

@note Value is read in as <tt>#String</tt>.

### Arguments

| Argument               | Description                                                                                            |
| ---------------------- | ------------------------------------------------------------------------------------------------------ |
| <tt>\*<i>+int</i></tt> | Indicate maximum number of characters to print. If max is > string length, prints up to string length. |
| <tt>\*_</tt>           | Indicates that maximum number of characters to print is passed in before string. Max is #I32_MAX       |
| <tt><i>+int</i></tt>   | Left side padding.                                                                                     |
| <tt><i>-int</i></tt>   | Right side padding.                                                                                    |
| <tt>u</tt>             | Characters are printed as upper-case.                                                                  |
| <tt>l</tt>             | Characters are printed as lower-case.                                                                  |

UTF-8 Path
----------

```
{p}
```

### Description

Write a <tt>#Path</tt>.

Only writes up to valid max path (4096 characters).

@note Value is read in as <tt>#Path</tt>.

### Arguments

| Argument               | Description                                                                                            |
| ---------------------- | ------------------------------------------------------------------------------------------------------ |
| <tt>\*<i>+int</i></tt> | Indicate maximum number of characters to print. If max is > string length, prints up to string length. |
| <tt>\*_</tt>           | Indicates that maximum number of characters to print is passed in before string. Max is #I32_MAX       |
| <tt><i>+int</i></tt>   | Left side padding.                                                                                     |
| <tt><i>-int</i></tt>   | Right side padding.                                                                                    |
| <tt>u</tt>             | Characters are printed as upper-case.                                                                  |
| <tt>l</tt>             | Characters are printed as lower-case.                                                                  |
| <tt>p</tt>             | Replace path separators with system separator.                                                         |
| <tt>c</tt>             | Output canonical path instead of relative path.                                                        |

Time Split
----------

```
{t}
```

### Description

Write a <tt>#TimeSplit</tt>.

When no time formatting is provided, uses default time formatting.

@note Value is read in as <tt>#TimeSplit</tt>.

### Arguments

| Argument                          | Description                                                                              |
| --------------------------------- | ---------------------------------------------------------------------------------------- |
| <tt>'<i>time formatting</i>'</tt> | See [time formatting](./FORMAT_TIME.md).                                                 |
| <tt>\*</tt>                       | Indicates that time formatting is provided as a <tt>const char\*</tt> before time split. |
| <tt><i>+int</i></tt>              | Left side padding.                                                                       |
| <tt><i>-int</i></tt>              | Right side padding.                                                                      |

Any Formatter
----------------

```
{a,...}
```

### Description

Format anything.

Requires function pointer and void pointer to be passed in.

Arguments are passed to formatter with surrounding \{\} and 'a' stripped.

Example of how arguments are passed:
```
{a,b,400,87}
```
becomes:
```
b,400,87
```

Full example:

```cpp
struct Foo {
    int bar;
};
struct Foo foo = { .bar = 10 };

usize foo_formatter(
    StreamBytesFN* stream, void* target,
    usize args_len, const char* args_ptr, const void* param
) {
    /* 'b,f' */
    String args = string_new( args_len, args_ptr );

    b32 binary = false, full_width = false;

    while( !string_is_empty( args ) ) {
        String argument = args;
        string_find( argument, ',', &argument.len );
        args = string_advance_by( args, argument.len + 1 );

        argument = string_trim_surrounding_whitespace( argument );
        if( string_is_empty( argument ) || argument.len > 1 ) {
            continue;
        }

        switch( argument.cbuf[0] ) {
            case 'b': {
                binary = true;
            } break;
            case 'f': {
                full_width = true;
            } break;
            default:
                continue;
        }
    }

    struct Foo* foo_ptr = (struct Foo*)param;
    usize result = 0;

    struct IntFormatArguments args = {};
    args.flags = FMT_INT_BITDEPTH_32;

    if( binary ) {
        args.flags |= FMT_INT_BINARY;
    }
    if( full_width ) {
        args.flags |= FMT_INT_FULL_WIDTH;
    }

    result += stream_fmt_int( stream, target, 0, 1, &foo_ptr->bar, &args );

    return result;
}

println( "{a,b,f}", foo_formatter, &foo );
```

### Arguments

Arguments are passed to formatter function.

