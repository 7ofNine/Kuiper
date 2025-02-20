# Note dependence of 'sub_eph' on the 'lunar' library.  This is available
# at http://www.projectpluto.com/source.htm .

all: asc2eph.exe dump_eph.exe eph2asc.exe merge_de.exe sub_eph.exe testeph.exe

clean:
   del asc2eph.obj
   del asc2eph.exe
   del dump_eph.obj
   del dump_eph.exe
   del eph2asc.obj
   del eph2asc.exe
   del f_strtod.obj
   del jpleph.dll
   del jpleph.exp
   del jpleph.lib
   del jpleph.obj
   del lunar.lib
   del merge_de.obj
   del merge_de.exe
   del sub_eph.obj
   del sub_eph.exe
   del testeph.obj
   del testeph.exe
   del wjpleph.lib

eph2asc.exe:     eph2asc.obj wjpleph.lib
   wcl386 -zq -k20000 eph2asc.obj wjpleph.lib

dump_eph.exe:     dump_eph.obj wjpleph.lib
   wcl386 -zq -k20000 dump_eph.obj wjpleph.lib

testeph.exe:      testeph.obj wjpleph.lib
   wcl386 -zq -k20000 testeph.obj wjpleph.lib

sub_eph.exe:      sub_eph.obj wjpleph.lib
   wcl386 -zq -k20000 sub_eph.obj wjpleph.lib ../lib/wafuncs.lib

asc2eph.exe:          asc2eph.obj f_strtod.obj
   wcl386 -zq -k20000 asc2eph.obj f_strtod.obj

merge_de.exe: merge_de.obj wjpleph.lib
   wcl386 -zq -k20000 merge_de.obj wjpleph.lib

wjpleph.lib: jpleph.obj
   wlib -q wjpleph.lib +jpleph.obj

CFLAGS=-W4 -Ox -j -4r -s -zq -i=..\include

.cpp.obj:
   wcc386 $(CFLAGS) $<

jpleph.obj:

testeph.obj:

sub_eph.obj: sub_eph.cpp
   wcc386 $(CFLAGS) -DTEST_MAIN sub_eph.cpp

dump_eph.obj:

merge_de.obj:

asc2eph.obj:

