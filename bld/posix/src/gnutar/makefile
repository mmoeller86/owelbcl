name = gnutar
objs =  tar.obj create.obj extract.obj buffer.obj getoopt.obj &
        list.obj names.obj port.obj

# MSDOS is defined when compiling for DOSOS2.  Make sure it is set for
# any other configuration
!ifdef __UNIX__
extra_cppflags = -DUSG -DMSDOS -DNONAMES -DDEF_AR_FILE=\"ARCHIVE.TAR\" -DDEFBLOCKING=20
!else
extra_cppflags = -DUSG -DMSDOS -DNONAMES -DDEF_AR_FILE="ARCHIVE.TAR" -DDEFBLOCKING=20
!endif

!include ../makeone.mif
