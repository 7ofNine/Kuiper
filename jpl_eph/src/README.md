# jpl_eph

Details at http://www.projectpluto.com/jpl_eph.htm .

This code provides various utilities and functions to read,  use,  and manipulate JPL
DE ephemeris data.  The core functions were based on Piotr Dybczynski's C source code :

ftp://ftp.astro.amu.edu.pl/pub/jpleph/

though this version has been so heavily changed as to bear only a passing resemblance
to the original,  and a variety of utilities have been added.  This version :

- deals with JPL ephemerides in either byte order;
- handles all JPL ephemeris versions (at least up to DE-435),  without recompiling;
- handles errors gracefully;
- can be compiled and used on DOS/Windows,  Linux,  OS/X,  with various compilers;
- includes some optimizations for speed

This code has been under development for some years and is essentially "complete",
except that one may expect JPL to produce further ephemerides and that they may add
new features requiring some changes to this code.  (In recent years,  JPL ephems
have had TT-TDB data added,  and some ephemerides have broken the previous limit
of 400 ephemeris constants.  Older code won't work with either of these issues.)

On Linux,  run `make` to build the library and various test executables
and utilities.  (You can also do this with MinGW under Windows.)  In Linux,  you
can then run `make install` to put libraries in `/usr/local/lib` and some
include files in `/usr/local/include`.  (You will probably have to make that
`sudo make install`.)  For BSD,  and probably OS/X,  run `gmake CLANG=Y`
(GNU make,  with the clang compiler),  then `sudo gmake install`.

On Windows,  run `nmake -f vc.mak` with MSVC++.  Optionally,  add
`-BITS_32=Y` for 32-bit code.

Makefiles are also provided for the [OpenWATCOM](http://www.openwatcom.org) and
[Digital Mars](http://www.digitalmars.com) compilers for DOS/Windows.
