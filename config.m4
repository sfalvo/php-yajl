dnl $Id$
dnl config.m4 for extension boxwood

PHP_ARG_ENABLE(yajl, enables YAJL bindings,
               [  --enable-yajl,           Enable YAJL bindings])

if test "$PHP_YAJL" != "no"; then
  AC_DEFINE(HAVE_PHP_YAJL, 1, [Whether YAJL bindings are enabled])
  PHP_NEW_EXTENSION(yajl, php-yajl.c, $ext_shared)
fi

