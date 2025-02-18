# Required packages
ca-certificates, git, make, python3, openocd, picocom

# installation
```
g++ installer.cpp -o install # compile installer (in case you need it)
```

```
./install # install mik32Loader
```

```
./install reinstall # reinstall mik32Loader 
```

```
mik32Load uninstall # uninstall mik32Loader
```
# mik32Load
you can launch loader from any directory

to launch loader type "mik32Load" from any directory, this directory will be passed to C-CPP-builder

the mik32Loader remembers the latest settings and the entered flags, so if you just enter a command without flags or arguments, the build will go the same way as the last time

Type "read" in any place after "mik32Load" to launch picocom and enable terminal output:

```
mik32Load read # launching picocom
```

# Flags

-b [number] specify SERIAL_BOUDRATE for picocom

Every flag not listed above (except "-o, --CC, --CXX") will be passed as arguments to C-CPP-builder: https://github.com/SergantDornan/C-Cpp-builder, read about arguments there
