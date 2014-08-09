graphplan
=========

C++ implementation of graphplan. This is not meant to be efficient nor pretty right now.

Building
--------
graphplan uses the [MPC project](https://www.ociweb.com/products/mpc) to generate makefiles. 
Running `mwc.pl -type make graphplan.mwc` to generate the makefiles and then use `make` to generate the library and associated test binary. 
The library will be built in `./lib` and the `test_graphplan` will be built in `./bin`.
