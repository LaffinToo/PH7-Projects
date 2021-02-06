# PH7-Projects
PH7 - The embedded PHP - Tools, Scripts &amp; Extensions

## PH7
https://github.com/symisc/PH7

PH7 is a in-process software C library which implements a highly-efficient embeddable bytecode compiler and a virtual machine for the PHP programming language. In other words, PH7 is a PHP engine which allow the host application to compile and execute PHP scripts in-process. PH7 is to PHP what SQLite is to SQL.

PH7 implements most of the constructs introduced by the PHP 5.3 release such as heredoc, nowdoc, gotos, classes, anonymous functions, closures and so on and introduces very [powerful extensions](http://ph7.symisc.net/features.html) to the PHP programming language such as:


 * Function & Method Overloading.
 * Full Type Hinting.
 * Introducing comma expressions.
 * Introducing the eq and ne operators for strict string comparison.
 * Improved operators precedences.
 * Powerful OO subsystem.
 * Function arguments can take any complex expressions as their default values.
 * 64-bit integer arithmetic for all platforms.
 * Native UTF-8 support.
 * Written in ANSI C, thread-safe, full-reentrant; compile and run unmodified in any platform including restricted embedded devices with a C compiler.
 * Amalgamation: All C source code for PH7 are combined into a single source file.
 * Built with more 470 function including an XML parser (with namespace support), INI processor, CSV reader/writer, UTF-8 encoder/decoder, zip archive extractor, JSON encoder/decoder, random number/strings generator, native and efficient File IO for Windows and UNIX systems and many more without the need of any external library to link with.
 * PH7 is an Open-Source product.
 

As an embedded interpreter, it allows multiple interpreter states to coexist in the same program, without any interference between them. Programmatically, foreign functions in C/C++ can be added and values can be defined in the PHP environment. Being a quite small program, it is easy to comprehend, get to grips with, and use.

## PH7-Projects

These projects are mostly Tools, extensions, and scripts. So you will need PH7.

### Tools
- splitamalgamation - Splits the amalgamated source into its sepetate files. Making it easier to edit.

- interpreter - This is same interpreter that comes with PH7. Modified to be better suited as a shell scripting.
I have a few more ideas to implement.

### Extensions
- bitman - A bit manipulation library, some people code a "Hello World" extension, I code bitman.

- SQLite3 (WIP) - I think having sqlite would  complement PH7 nicely. trying to keep the syntax the same.

### Scripts
these work as shell scripts with the interpreter.

- linkdir.ph7 - Sometimes i code on android, and its a pain to edit, copy, test, copy back, .... so this creates softlinks of files and creates directories. so now i can use my favorite editor.

- fntolwr.ph7 - Sometimes i find an archive with code i want to look at. But filenames are all uppercase. This script converts them to lowercase.
- 
