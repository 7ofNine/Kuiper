# Note dependence of 'sub_eph' on the 'lunar' library.  This is available
# at http://www.projectpluto.com/source.htm .

all: asc2eph.exe dump_eph.exe eph2asc.exe merge_de.exe testeph.exe sub_eph.exe

COMMON_FLAGS=-nologo -W3 -EHsc -c -FD -D_CRT_SECURE_NO_WARNINGS

!ifdef BITS_32
LUNARLIBNAME=lunar32
LIBNAME=jpleph32.lib
RM=del
!else
LUNARLIBNAME=lunar64
LIBNAME=jpleph64.lib
RM=del
!endif

clean:
   $(RM) asc2eph.obj
   $(RM) asc2eph.exe
   $(RM) dump_eph.obj
   $(RM) dump_eph.exe
   $(RM) eph2asc.obj
   $(RM) eph2asc.exe
   $(RM) f_strtod.obj
   $(RM) jpleph.dll
   $(RM) jpleph.exp
   $(RM) $(LIBNAME)
   $(RM) jpleph.obj
   $(RM) merge_de.obj
   $(RM) merge_de.exe
   $(RM) sub_eph.obj
   $(RM) sub_eph.exe
   $(RM) testeph.obj
   $(RM) testeph.exe

testeph.exe:    testeph.obj $(LIBNAME)
   link /nologo testeph.obj $(LIBNAME)

merge_de.exe:   merge_de.obj $(LIBNAME)
   link /nologo merge_de.obj $(LIBNAME)

eph2asc.exe:    eph2asc.obj $(LIBNAME)
   link /nologo eph2asc.obj $(LIBNAME)

dump_eph.exe:   dump_eph.obj $(LIBNAME)
   link /nologo dump_eph.obj $(LIBNAME)

asc2eph.exe:    asc2eph.obj f_strtod.obj
   link /nologo asc2eph.obj f_strtod.obj

sub_eph.exe:    sub_eph.obj $(LIBNAME)
   link /nologo sub_eph.obj $(LIBNAME) $(LUNARLIBNAME).lib

$(LIBNAME): jpleph.obj
   $(RM) $(LIBNAME)
!ifdef DLL
   $(RM) jpleph.dll
   link /nologo /DLL /IMPLIB:jpleph.lib /DEF:jpleph.def jpleph.obj
!else
   lib /OUT:$(LIBNAME) jpleph.obj
!endif

CFLAGS=-O2 -MT $(COMMON_FLAGS)

jpleph.obj: jpleph.cpp
!ifdef DLL
   cl $(CFLAGS) -LD jpleph.cpp
!else
   cl $(CFLAGS)     jpleph.cpp
!endif

sub_eph.obj: sub_eph.cpp
   cl $(CFLAGS) -DTEST_MAIN sub_eph.cpp

.cpp.obj:
   cl $(CFLAGS) $<

install:
   copy jpleph.h   ..\myincl
   copy $(LIBNAME) ..\lib
