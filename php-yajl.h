#ifndef PHP_YAJL_H
#define PHP_YAJL_H

#define PHP_YAJL_VERSION "1.0"
#define PHP_YAJL_EXTNAME "php-yajl"

PHP_FUNCTION(yajl_new);
PHP_FUNCTION(yajl_dispose);
PHP_FUNCTION(yajl_parse);

PHP_FUNCTION(hello_world);
PHP_FUNCTION(add_two_numbers);
PHP_FUNCTION(my_call_back);

extern zend_module_entry php_yajl_module_entry;
#define phpext_yajl_ptr &hello_module_entry

#endif

