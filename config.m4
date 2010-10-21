dnl $Id$
dnl config.m4 for extension boxwood

PHP_ARG_ENABLE(yajl, enables YAJL bindings,
               [  --with-yajl[=DIR]     Include YAJL bindings])

if test "$PHP_YAJL" != "no"; then
  if test -r $PHP_YAJL/lib/libyajl_s.a; then
    YAJL_DIR=$PHP_YAJL
  else
      AC_MSG_CHECKING(for yajl in default path)
      for i in /usr/local /usr; do
        if test -r $i/lib/libyajl_s.a; then
          YAJL_DIR=$i
          AC_MSG_RESULT(found in $i)
        fi
      done
  fi

  if test -z "$YAJL_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please install YAJL 1.0.11 or later.)
  fi

  PHP_ADD_INCLUDE($YAJL_DIR/include)
  PHP_SUBST(YAJL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(yajl, $YAJL_DIR/lib, YAJL_SHARED_LIBADD)

  AC_DEFINE(HAVE_PHP_YAJL, 1, [Whether YAJL bindings are enabled])
  PHP_NEW_EXTENSION(yajl, php-yajl.c, $ext_shared)
fi

