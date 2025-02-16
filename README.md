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

Everything you type after mik32Load will be passed as arguments to C-CPP-builder: https://github.com/SergantDornan/C-Cpp-builder, read about arguments there
