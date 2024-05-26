# Misc. unittests

This repository contains just misc. unittests.
The unittests are based on googletest which is referenced as git
submodule.

## Build and execute the tests on \*NIX

Initialize submodule once after clone.
```
git submodule update --init
```

```
cmake -S . -B build
cd build
make
make test
```
