- [Known issues](#known-issues)

There are 3 C++ samples: [Benchmark](benchmark), [Verify](verify) and [RuntimeKey](runtimeKey). Check [here](benchmark/README.md), [here](verify/README.md) and [here](runtimeKey/README.md) on how to build and use them.

<a name="known-issues"></a>
# Known issues #
- On Linux you may get `[CompVSharedLib] Failed to load library with path=<...>libplugin_vino.so, Error: 0xffffffff`. Make sure to set `LD_LIBRARY_PATH` to add binaries folder to help the loader find all dependencies. You can also run `ldd libplugin_vino.so` to see which libraries are missing.
- On Linux you may get `'GLIBC_2.27' not found (required by <...>)`. This message means you're using an old glibc version. Update glibc or your OS to Ubuntu 18, Debian Buster... You can check your actual version by running `ldd --version`. 
