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

## Build for release

```
cmake -DCMAKE_BUILD_TYPE=Release .
make clean
make
```

## Debugging

This section describes how to debug the game using `gdbgui`. If you don't have
it, run `pip install gdbgui`.

Ubuntu has an annoying default where it doesn't let you debug applications by
connection to a process ID. Turn that off when you want to debug programs.

```sh
sudo sh << END
echo 0 > /proc/sys/kernel/yama/ptrace_scope
END
```

Build the project in debug mode and start the executable running.

```
cmake -DCMAKE_BUILD_TYPE=Debug .
make clean
make
./mechmission
```

Open another terminal now the game has started, and get the process ID for it.

```
pgrep -af ./mechmission
```

Now run `gdbgui` telling it to only show our sources and start from a sensible
place to begin with. Enter the PID you saw before into the GUI.

```
gdbgui --project "$PWD/src" mechmission
```
