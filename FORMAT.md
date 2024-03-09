String Formatting
=================

Corelib does not use the C standard library's format specifiers and instead defines its own specifiers.

Format specifiers follow this prototype:

```cpp

{specifier,arguments}

ex:

println( "{u32,X,4}", 10 ) -> " 0xA"

```

Data is always assumed to be passed in by value unless the * argument is used.

When two arguments are conflicting, the last provided argument will take precedence.

@note Arguments in [ ] indicate that they describe a C literal, not the actual argument.
@note ex: [+int] means positive integer.

## Parenthesis

```{{```

### Description

Write a literal open parenthesis.

## Null Terminator

```
{0}
```

### Description

Write a literal null character.

## Boolean

```
{b}
```

### Description

Write a <tt>#b32</tt>, true if non-zero, false if zero.

### Arguments

| Argument                   | Description                                                                                                             |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| <tt>*[+int, optional]</tt> | Indicates that value is passed in by pointer. Integer indicates how many values to write (if pointer is an array).      |
| <tt>*_</tt>                | Indicates that value is passed in by pointer. Number of items to print is passed in before pointer and must be an #i32. |
| <tt>[+int]</tt>            | Left side padding.                                                                                                      |
| <tt>[-int]</tt>            | Right side padding.                                                                                                     |
| <tt>b</tt>                 | Write boolean as binary 1 or 0 instead of true or false.                                                                |

## Character

```
{c}
```

### Description

Write a <tt>char</tt>.

### Arguments

| Argument                   | Description                                                                                                                 |
| -------------------------- | --------------------------------------------------------------------------------------------------------------------------- |
| <tt>*[+int, optional]</tt> | Indicates that value is passed in by pointer. Integer indicates how many values to write (if pointer is an array).          |
| <tt>*_</tt>                | Indicates that value is passed in by pointer. Number of items to print is passed in before pointer and must be an #i32.     |
| <tt>r[+int, optional]</tt> | Repeat character. If no integer is provided, repeats once.                                                                  |
| <tt>r_</tt>                | Repeat character. Number of times to repeat is passed in before character and must be an #i32. Cannot be used with * or *_. |
| <tt>[+int]</tt>            | Left side padding.                                                                                                          |
| <tt>[-int]</tt>            | Right side padding.                                                                                                         |
| <tt>u</tt>                 | Convert character to upper-case if it is lower-case.                                                                        |
| <tt>l</tt>                 | Convert character to lower-case if it is upper-case.                                                                        |

## Null-terminated String

```
{cc}
```

### Description

Write a <tt>const char*</tt> string.

### Arguments

| Argument                   | Description                                                                     |
| -------------------------- | ------------------------------------------------------------------------------- |
| <tt>*[+int]</tt>           | Only print up to given number of characters or null-terminator.                 |
| <tt>*_</tt>                | Same as above except provided as an argument before string and must be an #i32. |
| <tt>[+int]</tt>            | Left side padding.                                                              |
| <tt>[-int]</tt>            | Right side padding.                                                             |
| <tt>u</tt>                 | Convert string to upper-case.                                                   |
| <tt>l</tt>                 | Convert string to lower-case.                                                   |

## Float

```
{f} {f32} {f64}
```

### Description

Write an <tt>#f64</tt> (f32 gets promoted to f64 in varargs).

### Arguments

| Argument                   | Description                                                                                                             |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| <tt>*[+int, optional]</tt> | Indicates that value is passed in by pointer. Integer indicates how many values to write (if pointer is an array).      |
| <tt>*_</tt>                | Indicates that value is passed in by pointer. Number of items to print is passed in before pointer and must be an #i32. |
| <tt>[+int]</tt>            | Left side padding.                                                                                                      |
| <tt>[-int]</tt>            | Right side padding.                                                                                                     |
| <tt>0[+int]</tt>           | Left side padding, pad with zeroes instead of spaces.                                                                   |
| <tt>s</tt>                 | Separate every 3 whole number digits by a comma.                                                                        |
| <tt>m</tt>                 | Memory formatting. Assumes value is in bytes, converts to most convenient unit (ex: 1280.0f -> "1.25 KB")               |

## Vector

```
{v2} {v3} {v4}
```

### Description

Write either a <tt>#Vector2</tt> <tt>#Vector3</tt> or <tt>#Vector4</tt>.

### Arguments

| Argument                   | Description                                                                                                             |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| <tt>*[+int, optional]</tt> | Indicates that value is passed in by pointer. Integer indicates how many values to write (if pointer is an array).      |
| <tt>*_</tt>                | Indicates that value is passed in by pointer. Number of items to print is passed in before pointer and must be an #i32. |
| <tt>[+int]</tt>            | Left side padding.                                                                                                      |
| <tt>[-int]</tt>            | Right side padding.                                                                                                     |
| <tt>0[+int]</tt>           | Left side padding, pad with zeroes instead of spaces.                                                                   |
| <tt>s</tt>                 | Separate every 3 whole number digits by a comma.                                                                        |

## Integer

```
Signed:   {i} {i8} {i16} {i32} {i64} {isize}
Unsigned: {u} {u8} {u16} {u32} {u64} {usize}
```

### Description

Write <tt>#i8</tt>, <tt>#i16</tt>, <tt>#i32</tt>, <tt>#i64</tt>, <tt>#isize</tt>, 
<tt>#u8</tt>, <tt>#u16</tt>, <tt>#u32</tt>, <tt>#u64</tt> or <tt>#usize</tt>.

Specifier with no size after it is assumed to be a 32-bit integer.

### Arguments

| Argument                   | Description                                                                                                             |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| <tt>*[+int, optional]</tt> | Indicates that value is passed in by pointer. Integer indicates how many values to write (if pointer is an array).      |
| <tt>*_</tt>                | Indicates that value is passed in by pointer. Number of items to print is passed in before pointer and must be an #i32. |
| <tt>[+int]</tt>            | Left side padding.                                                                                                      |
| <tt>[-int]</tt>            | Right side padding.                                                                                                     |
| <tt>0[+int]</tt>           | Left side padding, pad with zeroes instead of spaces.                                                                   |
| <tt>x</tt>                 | Lower-case hexadecimal.                                                                                                 |
| <tt>X</tt>                 | Upper-case hexadecimal.                                                                                                 |
| <tt>b</tt>                 | Binary formatting.                                                                                                      |
| <tt>f</tt>                 | Write to full integer width (hex and binary only).                                                                      |
| <tt>s</tt>                 | Binary: Separate every 8 digits by a tick. Decimal: every 3 digits by a comma. Hex: every 4 digits by a tick.           |

## String

```
{s}
```

### Description

Write a <tt>#String</tt>.

### Arguments

| Argument                   | Description                                                     |
| -------------------------- | --------------------------------------------------------------- |
| <tt>*[+int]</tt>           | Only print up to given number of characters or null-terminator. |
| <tt>[+int]</tt>            | Left side padding.                                              |
| <tt>[-int]</tt>            | Right side padding.                                             |
| <tt>u</tt>                 | Convert string to upper-case.                                   |
| <tt>l</tt>                 | Convert string to lower-case.                                   |

## Path

```
{p}
```

### Description

Write a <tt>#Path</tt>.

### Arguments

| Argument                   | Description                                                     |
| -------------------------- | --------------------------------------------------------------- |
| <tt>*[+int]</tt>           | Only print up to given number of characters or null-terminator. |
| <tt>[+int]</tt>            | Left side padding.                                              |
| <tt>[-int]</tt>            | Right side padding.                                             |

