AC_PREREQ(2.52)

AC_INIT(dplayer, 0.1)

dnl versions of gstreamer
GST_MAJORMINOR=0.10
GST_REQUIRED=0.10.0

dnl versions of GTK+
GTK_MAJORMINOR=2.0
GTK_REQUIRED=2.0.0

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([
Makefile 
src/Makefile 
doc/Makefile 
pixmaps/Makefile 
dplayer.desktop])

dnl AM_MAINTAINER_MODE provides the option to enable maintainer mode
AM_MAINTAINER_MODE

dnl check for tools
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

dnl Add parameters for aclocal
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4")

dnl Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)

dnl Give error and exit if we don't have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
  AC_MSG_ERROR(you need to have pkgconfig installed !)
fi

PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GST_REQUIRED, HAVE_GST=yes, HAVE_GST=no)

dnl Give error and exit if we don't have gstreamer
if test "x$HAVE_GST" = "xno"; then
  AC_MSG_ERROR(you need gstreamer development packages installed !)
fi

dnl append GST_ERROR cflags to GST_CFLAGS
GST_CFLAGS="$GST_CFLAGS $GST_ERROR"

dnl make GST_CFLAGS and GST_LIBS available
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)

dnl make GST_MAJORMINOR available in Makefile.am
AC_SUBST(GST_MAJORMINOR)

PKG_CHECK_MODULES(OMAPGTK, omapgtk-1.0, have_omapgtk=1, have_omapgtk=0)
AC_DEFINE_UNQUOTED(HAVE_OMAPGTK, $have_omapgtk, [OmapGtk support])
if test $have_omapgtk = 1; then
echo OmapGtk wil be used
PKG_CHECK_MODULES(GTK, omapgtk-1.0 gtk+-$GTK_MAJORMINOR >= $GTK_REQUIRED, HAVE_GTK=yes, HAVE_GTK=no)
else
echo OmapGtk is not installed
PKG_CHECK_MODULES(GTK, gtk+-$GTK_MAJORMINOR >= $GTK_REQUIRED, HAVE_GTK=yes, HAVE_GTK=no)
fi

dnl Give error and exit if we don't have GTK+
if test "x$HAVE_GTK" = "xno"; then
  AC_MSG_ERROR(you need GTK+ development packages installed !)
fi

dnl make GTK_CFLAGS and GTK_LIBS available
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

dnl make GTK_MAJORMINOR available in Makefile.am
AC_SUBST(GTK_MAJORMINOR)

AC_OUTPUT
