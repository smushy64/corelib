Time Formatting
===============

Format arguments are case-insensitive.

Any unrecognized character is printed as-is.

Format specifiers are surrounded by square brackets like so:

<tt>\[specifier\]</tt>

Default formatting is:

```cpp
"[mm]/[ddd]/[yyyy] [h]:[m]:[s] [hm]"
```

| Argument       | Description                               |
| -------------- | ----------------------------------------- |
| <tt>yy   </tt> | Two digit year. Always zero padded.       |
| <tt>yyyy </tt> | Four digit year.                          |
| <tt>mm   </tt> | Two digit month. Always zero padded.      |
| <tt>mmm  </tt> | Three character short month name.         |
| <tt>mmmm </tt> | Full month name.                          |
| <tt>d    </tt> | Day of week. Starts Sunday, zero padded.  |
| <tt>dd   </tt> | Name of day of week.                      |
| <tt>ddd  </tt> | Day of month.                             |
| <tt>dddd </tt> | Day of year. Always zero padded.          |
| <tt>h    </tt> | Hour in 12-hour format.                   |
| <tt>hh   </tt> | Hour in 24-hour format.                   |
| <tt>hm   </tt> | 12-hour AM/PM. (upper-case)               |
| <tt>hml  </tt> | 12-hour am/pm. (lower-case)               |
| <tt>m    </tt> | Minute. Always zero padded.               |
| <tt>s    </tt> | Seconds. Always zero padded.              |

Use double open bracket ([[) to write bracket literal.

Example, assuming the current day of the week is Monday:

```cpp
"[dd][[ Message ]"
```

Output:

```sh
'Monday[ Message ]'
```

