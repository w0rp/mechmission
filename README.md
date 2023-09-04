# mechmission

## Build

To build the project, make sure source libraries are loaded by updating git
submodules.

```
git submodule update --init
```

Ensure that ncurses is installed on the system.

## Linting

Use `clangd` and make sure `clang-tidy-17` is installed to check code.

Checks can be run on the whole project like so:

```
run-clang-tidy-17 -header-filter '.*\.hpp' -quiet src
```
