# c-autotools-template-small

This is template for new C projects using [GNU
Autotools](https://www.gnu.org/software/automake/manual/html_node/index.html),
[Unity Test](http://www.throwtheswitch.org/unity), and
[CMock](http://www.throwtheswitch.org/cmock), with one C file per module.

I originally wrote
[dansanderson/c-autotools-template](https://github.com/dansanderson/c-autotools-template)
to support a notion of a C module with multiple source files, and soon felt
like I needed another layer of code generation to manage the `Makefile.am`.
That's fine, but it was a little unsatisfying to not take better advantage of
GNU Autotools to describe the project.

It was the multi-file modules bit that felt like going against the grain, so I
thought I'd try another template that assumes one C compilation unit (one `.c`
file) per "module." This is suitable for many projects, and it felt premature to
assume otherwise by default.

Project maintainers need Ruby 2.x installed for the Unity Test and CMock code
generators. The source distribution can be built on any POSIX-compliant system
without Ruby.

**Note:** Be sure to clone this repo (or your own project repo based on this
repo) with `git clone --recurse-submodules` so that the CMock testing library
in `third-party/` and its submodules are also instaled. If you omitted this
when cloning, run this to finish the process: `git submodule update --init
--recursive`

## Plain C Modules

A _module_ is defined as a C header file (`.h`) that declares the public
interface, and a C source file (`.c`) that implements it. These files live in
`src/`, alongside other modules.

A _program_ is a C source file (`.c`) that defines `int main(int argc, char
**argv)`. It does not have a corresponding header file because it does not
offer a public interface to other modules.

Because a program might link multiple modules, each module needs to use
globally unique names for its interface (functions, types, global storage). The
canonical way to do this is to use the module name as a prefix for the
interface name: `module_interface`

All functions and storage in a module that are not exposed to other modules in
the interface must be declared `static` in the `.c` file. These are not visible
outside of the module source file, and so do not need a naming convention.

## Tests and mocks

Each module should have a test suite that exercises its public interface. This
template uses [Unity Test](http://www.throwtheswitch.org/unity) to generate
test runner code and provide assertion functions. See the [Unity Assertions Reference](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md).

A module's test suite is a C source file in `tests/` whose filename begins with
`test_`. It contains one or more test functions, each one with a name beginning
with `test_`. It should have a `void` parameter specification and a `void`
return type. It is recommended to include the name of the function under test,
the condition of the test, and the expected result in the name of the
`test_...()` function, spelled CamelCase and delimited by underscores.

A test suite can optionally define `void setUp(void)` and `void
tearDown(void)`, to be called before and after each test, respectively.

Each test suite is typically linked with the module under test, along with
_mock modules_ for each dependency of the module under test, generated with
[CMock](http://www.throwtheswitch.org/cmock). Every call that the function
under test makes to a dependency module must be declared with a [CMock
expectation](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md).
The actual dependency function is not called.

```c
#include "executor/executor.h"
#include "mock_cfgfile.h"
#include "unity.h"

void test_Square_UsesExampleTwo(void) {
  cfgfile_func_ExpectAndReturn(7, 49);
  int result = executor_doit(7);
  TEST_ASSERT_EQUAL_INT(49, result);
}
```

## configure.ac and Makefile.am

[GNU
Autotools](https://www.gnu.org/software/automake/manual/html_node/index.html)
is a suite of packaging tools designed for open source Unix software. It is a
decades-old de facto standard, and is often used for macOS and Windows software
as well. It is typically used to produce a _source distribution_ that is
delivered directly to an end user or system administrator. The user runs
several commands to compile and install the package:

```text
tar xzf coolsoftware-1.0.tar.gz
cd coolsoftware-1.0.tar.gz
./configure
make
sudo make install
```

A GNU Autotools project provides two configuration files: `configure.ac` and
`Makefile.am`. These files describe the requirements and build steps for the
software. The project developer (that's you) uses the command
`autoreconf --install` to set up the environment based on these files, then
runs the `./configure` and `make` steps similar to how an end user would.

This template provides starter code for these files, similar to what's
described in a typical [Autotools
tutorial](https://www.lrde.epita.fr/~adl/autotools.html). You will customize
and extend these files for your project.

## Defining programs in Makefile.am

To create a new program in the project:

1. Create a C source file (`.c`) in `src/` with the name of the program containing a
   definition for the `int main(int argc, char **argv)` function.
2. In `Makefile.am`, add the name of the program to `bin_PROGRAMS`.
3. In `Makefile.am`, create a new `myapp_SOURCES` definition, where `myapp` is
   the name of the program. Its value is the list of `.c` source files for the
   program and all of the modules in the project that the program uses.

The example program named `myapp` is defined in `Makefile.am` as follows:

```makefile
bin_PROGRAMS = myapp

myapp_SOURCES = \
	$(top_srcdir)/src/myapp.c \
	$(top_srcdir)/src/cfgfile.c \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/reporter.c
```

## Defining modules in Makefile.am

To create a new module in the project:

1. Create a C source file (`.c`) in `src/` with the name of the module, and a
   corresponding C header file (`.h`).
2. Create a C source file in `tests/` with the name `test_` followed by the
   name of the module and a `.c` extension.
3. In `Makefile.am`, add an entry to `check_PROGRAMS` with the name
   `tests/runners/test_modname` where `modname` is the name of the module.
4. In `Makefile.am`, create a new `tests_runners_test_modname_SOURCES`
   definition where `modname` is the name of the module. Its value consists of:
    1. Mock source files (`tests/mocks/mock_xxx.c` and `.h`) for other modules
       that this module depends on that CMock will generate automatically
    2. A test runner source file that Unity Test will generate automatically
    3. The module test source file
    4. The module's source and header files
    5. `$(CMOCK_SRCS)`

The `SOURCES` order matters: the mocks must be first and the runner second, so
the generators run in the correct order.

The example module named `executor` is defined in `Makefile.am` as follows,
with a mocked dependency on the `cfgfile` module:

```makefile
tests_runners_test_executor_SOURCES = \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h \
	tests/runners/runner_test_executor.c \
	$(top_srcdir)/tests/test_executor.c \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/executor.h \
	$(CMOCK_SRCS)
```

## Using Automake

The complete sequence of commands to build all programs is as follows:

```text
autoreconf --install
./configure
make
```

In the example template, this builds the `myapp` program in the project root
directory.

Typically, you only need to use the `make` command after running these commands
for the first time in your project workspace. You only need to run `autoconf`
if `./configure` does not exist, and `./configure` is `Makefile` doesn't exist.
GNU Autotools generates the `Makefile` such that if you change `configure.ac`
or `Makefile.am`, running `make` will regenerate files as needed. If something
seems to be wrong, you can re-run `autoreconf --install` and `./configure` at
any time.

Autotools produces a significant number of auxiliary files in the project
workspace. The template's `.gitignore` file is set up to prevent these from
being committed to your source repo. You may need to add entries to
`.gitignore` if you extend `configure.ac` with new features (such as Libtool).

To clean up the result of `make`:

```text
make clean
```

To clean up the result of `./configure` and `make`:

```text
make distclean
```

By design, there is no built-in way to clean up all of the files generated by
`autoreconf`. I include a script to delete everything ignored by `.gitignore`,
and clean generated files out of `third-party/`. To fully reset the workspace:

```text
python3 scripts/superclean.py
```

To run all unit tests:

```text
make check
```

To run a single unit test:

```text
make check TESTS='tests/runners/test_cfgfile'
```

To build a single tool or a single test runner (without running it):

```text
make myapp

make tests/runners/test_cfgfile
```

The test runners are just programs. You can use them with your debugger. This
template includes VSCode configuration for running a test suite in a debugger
from its source file ("Debug test suite").

To make the source distribution:

```text
make distcheck
```

This builds `myapp-0.1.tar.gz` in the project root directory. This filename is
derived from the first line of `configure.ac`.

It's a good idea to run `make distcheck` after making major changes to
`Makefile.am` just to confirm that the Makefile rules work in isolation of the
current state of the project workspace.

## License

This template and related tools are released under [The
Unlicense](https://unlicense.org/). See [LICENSE](./LICENSE) for complete text.

You are _not_ required to use this license for your project. Replace the
`LICENSE` file with whatever is appropriate.

## Note from the author

My larger
[c-autotools-template](https://github.com/dansanderson/c-autotools-template)
uses a tool (`makemake.py`) that generates `Makefile.am` from proprietary
configuration files. I have mixed feelings about contributing another
proprietary module system to the world, but sometimes it's just the easiest way
to produce the desired result.

I didn't feel right suggesting that that's the best way to do things for all
projects and wanted a functional example that didn't rely on another layer of
code generation. Single-file modules are very common in C projects, and are a
fine place to start.

For what it's worth, I'm also uneasy with how this small template omits a
formalized statement of module dependencies entirely. A program must list every
module in its dependency tree as as source, even if the `main()` source file
for the program only refers to one or two. Each test suite must also list
immediate dependencies as mocks. Overall that's not too bad, and you can use
Automake variables (similar to Makefile variables) to reduce the amount of text
involved.

Feedback is welcome! If you have ideas for how this can be improved, fixed, or
made more generally useful, please [file an
issue](https://github.com/dansanderson/c-autotools-template-small/issues).
[Pull
requests](https://github.com/dansanderson/c-autotools-template-small/pulls) are
also welcome, though please pardon me if I take a while to respond.

Thanks!

— Dan (contact@dansanderson.com)
