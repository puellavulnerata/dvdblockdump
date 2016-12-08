AC_INIT([dvdblockdump],[0.1.0])
AC_CONFIG_SRCDIR([dvdblockdump.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE()
AM_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([libdvdread], [dvdread])

AC_LANG(C)

# Save CFLAGS/LDFLAGS/LIBS

saved_CFLAGS=${CFLAGS}
saved_LDFLAGS=${LDFLAGS}
saved_LIBS=${LIBS}

# Update CFLAGS/LDFLAGS/LIBS for dvdread checks

CFLAGS="${CFLAGS} ${libdvdread_CFLAGS}"
LDFLAGS="${LDFLAGS} ${libdvdread_LDFLAGS}"
LIBS="${LIBS} ${libdvdread_LIBS}"

# Check for extra libdvdread functions we need
AC_CACHE_CHECK([for DVDGetMaxLB() in -ldvdread],
[dvdblockdump_cv_have_dvd_get_max_lb],
[AC_LINK_IFELSE([AC_LANG_SOURCE([
#include <stdint.h>

#include <dvdread/dvd_reader.h>

int main(int argc, char **argv) {
  uint32_t (*fp)(dvd_reader_t *);

  fp = DVDGetMaxLB;

  return (fp != 0) ? 0 : -1;
}
])],
[dvdblockdump_cv_have_dvd_get_max_lb=yes],
[dvdblockdump_cv_have_dvd_get_max_lb=no])])

AC_CACHE_CHECK([for DVDReadRawBlocks() in -ldvdread],
[dvdblockdump_cv_have_dvd_read_raw_blocks],
[AC_LINK_IFELSE([AC_LANG_SOURCE([
#include <stdint.h>

#include <dvdread/dvd_reader.h>

int main(int argc, char **argv) {
  uint32_t (*fp)(dvd_reader_t *, unsigned char *, uint32_t, uint32_t, int);

  fp = DVDReadRawBlocks;

  return (fp != 0) ? 0 : -1;
}
])],
[dvdblockdump_cv_have_dvd_read_raw_blocks=yes],
[dvdblockdump_cv_have_dvd_read_raw_blocks=no])])

# Restore CFLAGS/LDFLAGS/LIBS

CFLAGS=${saved_CFLAGS}
LDFLAGS=${saved_LDFLAGS}
LIBS=${saved_LIBS}

AS_IF([test x$dvdblockdump_cv_have_dvd_get_max_lb = xyes], [],
[AC_MSG_ERROR([A libdvdread which supports DVDGetMaxLB() is required.])])

AS_IF([test x$dvdblockdump_cv_have_dvd_read_raw_blocks = xyes], [],
[AC_MSG_ERROR([A libdvdread which supports DVDReadRawBlocks() is required.])])

AC_OUTPUT(Makefile)
