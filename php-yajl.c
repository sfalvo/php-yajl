#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php-yajl.h"

#include <yajl/yajl_parse.h>

/*
 * When PHP binds functions to C code, it uses this table to dispatch from.
 */
static function_entry yajl_functions[] = {
    PHP_FE(yajl_new, NULL)
    PHP_FE(yajl_dispose, NULL)
    PHP_FE(yajl_parse, NULL)

    PHP_FE(hello_world, NULL)
    PHP_FE(add_two_numbers, NULL)
    PHP_FE(my_call_back, NULL)

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

typedef struct php_yajl_record {
    yajl_handle                 yajl;
    zval *                      php_ctx;
    zend_fcall_info             fci;
    zend_fcall_info_cache       fci_cache;
} php_yajl_record;

/* {{{ dummy callback for testing purposes. */
static int yajl_callback_null(void *yajl_ctx) {
    php_yajl_record *instance = (php_yajl_record *)yajl_ctx;
    zval *return_handle = NULL;
    HashTable *hash;

    /* Tell PHP where to put the return value */
    php_printf("AAA\n");
    php_printf("%p\n", &instance->fci);
    instance->fci.retval_ptr_ptr = &return_handle;

    /* Bundle all the input parameters into an array. */
    php_printf("BBB\n");
    instance->fci.params = safe_emalloc(sizeof(zval *), 1, 0);
    instance->fci.param_count = 1;
    instance->fci.params[0] = &instance->php_ctx;
    php_printf("BBB\n");

    /* Call the supplied function. */
    php_printf("CCC\n%p\n%p\n", instance, instance);
    zend_call_function(&instance->fci, &instance->fci_cache TSRMLS_CC);

    /* If the allocation succeeded, free the parameter array. */
    if(instance->fci.params) {
        efree(instance->fci.params);
    }

    /* Without this, PHP seems to want to abort trap on me. */
    return 1;
}

static int yajl_callback_boolean(void *ctx, int v)                                                  { return 1; }
static int yajl_callback_integer(void *ctx, long v)                                                 { return 1; }
static int yajl_callback_double(void *ctx, double v)                                                { return 1; }
static int yajl_callback_string(void *ctx, const unsigned char *s, const unsigned int sLength)      { return 1; }
static int yajl_callback_start_map(void *ctx)                                                       { return 1; }
static int yajl_callback_map_key(void *ctx, const unsigned char *k, const unsigned int kLength)     { return 1; }
static int yajl_callback_end_map(void *ctx)                                                         { return 1; }
static int yajl_callback_start_array(void *ctx)                                                     { return 1; }
static int yajl_callback_end_array(void *ctx)                                                       { return 1; }
/* }}} */

static yajl_callbacks callbacks = {
    yajl_callback_null,
    yajl_callback_boolean,
    yajl_callback_integer,
    yajl_callback_double,
    NULL,
    yajl_callback_string,
    yajl_callback_start_map,
    yajl_callback_map_key,
    yajl_callback_end_map,
    yajl_callback_start_array,
    yajl_callback_end_array
};

PHP_FUNCTION(yajl_new) {
    yajl_handle hYajl;                          /* YAJL's handle to its parser state */
    yajl_parser_config cfg = { 0, 0 };          /* Configuration options affecting exactly how it parses. */
    long php_config_allowComments;              /* YAJL configuration fields broken out for easier code maintenance */
    long php_config_checkUTF8;                  /* YAJL configuration fields broken out for easier code maintenance */
    HashTable *hash;                            /* Temporary used for parsing PHP arrays */
    zval *php_ctx;                              /* App-specific context zval; not interpreted by us or by YAJL. */
    php_yajl_record *instance;                  /* Stores a YAJL Parser/PHP callbacks mapping */
    zend_fcall_info fci;                        /* Callback function info block -- black magic. */
    zend_fcall_info_cache fcc;                  /* Callback function cache -- black magic. */

    /* Parse supplied parameters */
php_printf("AA\n");
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llzf", &php_config_allowComments, &php_config_checkUTF8, &php_ctx, &fci, &fcc)) {
        RETURN_NULL();
    }

php_printf("AA\n");
    cfg.allowComments   = php_config_allowComments;
    cfg.checkUTF8       = php_config_checkUTF8;

    /* Attempt to instantiate a PHP/YAJL association */
php_printf("AA\n");
    instance = (php_yajl_record *)(emalloc(sizeof(php_yajl_record)));
    if(!instance) goto no_instance;

    /* Open the YAJL instance. */
php_printf("AA\n");
    hYajl = yajl_alloc(&callbacks, &cfg, NULL, instance);
    if(!hYajl) goto no_yajl;

    /* Create and return an association between the YAJL instance and the supplied callbacks. */
php_printf("AA\n");
    instance->yajl          = hYajl;
    instance->php_ctx       = php_ctx;
    memcpy(&instance->fci, &fci, sizeof(fci));
    memcpy(&instance->fci_cache, &fcc, sizeof(fcc));
    if(php_ctx) Z_ADDREF_P(php_ctx);

php_printf("AA\n");
    RETURN_LONG((long)instance);

    /* Error handling */
no_yajl:
    if(instance) efree(instance);
no_instance:
    RETURN_LONG(0);
}

PHP_FUNCTION(yajl_parse) {
    php_yajl_record *instance;
    char *input_string;
    int input_length;
    int r;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", (long *)&instance, &input_string, &input_length)) {
        RETURN_NULL();
    }

    r = yajl_parse(instance->yajl, input_string, input_length);

    RETURN_LONG(r);
}

PHP_FUNCTION(yajl_dispose) {
    php_yajl_record *instance;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", (long *)&instance)) {
        RETURN_NULL();
    }

    if(instance) {
        if(instance->php_ctx)       Z_DELREF_P(instance->php_ctx);
        if(instance->yajl)          yajl_free(instance->yajl);
        efree(instance);
    }
    RETURN_NULL();
}

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

PHP_FUNCTION(my_call_back) {
    zval *return_handle = NULL;
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    zval *the_parameter;
    char tmp[30];

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &fci, &fci_cache)) {
        RETURN_NULL();
    }

    /* Tell our function call execution context where to put its return results. */
    fci.retval_ptr_ptr = &return_handle;

    /* Synthesize its input parameters. */
    memset(tmp, 0, 30);
    tmp[0] = '0';
    tmp[1] = 'x';
    sprintf(&tmp[2], "%016lX", (unsigned long)tmp);
    ZVAL_STRING(the_parameter, tmp, 0);

    /* Bundle all the input parameters into an array. */
    fci.params = safe_emalloc(sizeof(zval *), 1, 0);
    fci.param_count = 1;
    fci.params[0] = &the_parameter;

    /* Call the supplied function. */
    zend_call_function(&fci, &fci_cache TSRMLS_CC);

    /* If the allocation succeeded, free the parameter array. */
    if(fci.params) {
        efree(fci.params);
    }

    /* Without this, PHP seems to want to abort trap on me. */
    RETURN_NULL();
}

