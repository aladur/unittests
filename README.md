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

## Build and execute the tests on Windows

Execute the following steps in git bash:

Initialize submodule once after clone.
```
git submodule update --init
```

Configure, generate and make executable.

```
./rebuild.sh
```

Execute the unittests depending on the configuration and platform.

```
buildWin32\Release\unittests.exe
buildWin32\Debug\unittests.exe
buildx64\Release\unittests.exe
buildx64\Debug\unittests.exe
```

