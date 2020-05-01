# Install

# Download Valgrind

```bash
$ git clone --depth=1 https://sourceware.org/git/valgrind.git
$ export PATH_TO_VALGRING=$PWD/valgrind
```
# Download Vericheck

## Clone Vericheck into Valgrind

```bash
$ cd $PATH_TO_VALGRING
$ git clone https://yohanchatelain/vericheck.git
```

or copy the project into Valgrind

```bash
$ cd $PATH_TO_VALGRIND
$ mv $PATH_TO_VERICHECK/vericheck .
```

## Apply the patch to compile Vericheck in Valgrind

This patch allows to build vericheck during the valgrind build

```bash
$ cd $PATH_TO_VALGRIND
$ git apply vericheck/vericheck.install.patch
```

# Install Valgrind

This install valgrind and vericheck

```bash
$ cd $PATH_TO_VALGRIND
$ ./autogen.sh
$ ./configure --prefix=$LOCAL_PATH
$ make install
```
