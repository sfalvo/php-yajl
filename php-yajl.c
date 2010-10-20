#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php-yajl.h"

/*
 * When PHP binds functions to C code, it uses this table to dispatch from.
 */
static function_entry yajl_functions[] = {
    PHP_FE(hello_world, NULL)
    PHP_FE(add_two_numbers, NULL)
    {NULL, NULL, NULL}
};

/*
 * This structure is used by PHP to discover the existence of the extension and to learn bits of knowledge about it.
 */
zend_module_entry php_yajl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_YAJL_EXTNAME,
    yajl_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_YAJL_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_YAJL
ZEND_GET_MODULE(php_yajl)
#endif

/*
 * These are the actual function entry points.
 */

PHP_FUNCTION(hello_world) {
    RETURN_STRING("Hello world", 1);
}

PHP_FUNCTION(add_two_numbers) {
    long a, b;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &a, &b)) {
        RETURN_NULL();
    }

    RETURN_LONG(a+b);
}

