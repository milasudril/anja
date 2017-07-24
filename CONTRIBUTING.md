Contributing to Anja
====================

Thank you for your interest in making Anja the most useful live sample player ever.
Before continuing, take your time to read through these guidelines on how to proceed.

Reporting bugs
--------------
Most software contains bugs, and this is likely to be true for Anja as well. If
you find any bug you can currently post an issue to the GitHub issue tracker [1].
When posting a bug record, include the following information:

 1. The exact procedure to reproduce the bad behaviour

 2. The session file used, together with all waveform files. This may
    require some online file sharing service.

 3. Your hardware specification. Do you have AVX or not? How much RAM do you have?

 4. If you experience strange look and feel, state what GTK+3 theme you are using.


Feature requests
----------------
Anja has been written to satisfy the needs of the developer. As the developer has
not seen the entire world, you may need a feature that the developer has not though
of. Feature requests can also be posted to the GitHub issue tracker [1]. A feature
request should include

 1. A short description of the missing feature

 2. What it is good for

 3. Some kind of UI concept such as how to access it and how to set parameters

Any feature within the scope of "playing sound effects" can be requested. However,
they will only be implemented if it can be done within lowlatency realtime
constraints. This means that **a feature request that says "add PulseAudio support"
will be rejected** unless PulseAudio gets a better realtime behaviour, but it is
fine to suggest a seamingly more radical change that the UI should be ported to
another API such as *modern* versions of Qt, the classic GTK+-2.0 or the upcoming
GTK+-4.0.


Code contributions
------------------
As Anja is free software, you are welcome to improve Anja and make a pull request.
To do so

 1. Fork the project

 2. Clone your fork

 3. Compile it

 4. Do your changes

 5. Test it

 6. Push your fork

 7. Make a pull request

Before doing anything, you will probably benefit from reading the source code 
overview in the manual. In addition to that text, here are some coding practices
that are followed withinthis project. Items are not mentioned in any particular
order.

 * Declare each variable separately

   Good:

       float foo;
       float bar;

   OK:

       float foo; float bar;

   Bad:

       float foo, bar;

 * Do not use any naked resource allocating function or operator

 * Follow "rule of the zero", or "rule of the five"

 * If you find and correct a bug, and correct it, add more comments to the
   location in the code where the bug appeared. Since there was a bug, it is
   likely that this part of the code was harder to understand.

 * Make constructors explicit

 * Prefer ISO C or C++ function over system specific functions, but be careful with
   C99 functions, as it is possible that C99 is not supported out-of-the-box
   everywhere (MinGW links to the old MSVCRT which is C89 only + Microsoft
   specific extensions [2]).

 * Remember when you are in a realtime context. When you hacking on a function
   that is called from a realtime context, it must not call malloc/free or any
   blocking function. Also, minimize the use of trancendental functions, and
   rational powers.

 * Use immutable state if and only if keeping the object would not be cheaper in
   terms of asymptotic complexity

 * Use `long` if and only if a function from an external library uses it. If you
   want a pointer-sized integer, user `intptr_t` or `size_t`. If it is another
   integer you should use `int` or, if larger range is required, `long long`.

 * Use tabs for indentation, except in markdown files

 * Surround every block with braces

   Good:

       if(foo)
           {doStuff();}

   Bad:

       if(foo)
           doStuff();

[1] https://github.com/milasudril/anja/issues

[2] http://www.mingw.org/wiki/c99
