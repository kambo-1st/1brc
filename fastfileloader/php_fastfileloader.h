/* fastfileloader extension for PHP */

#ifndef PHP_FASTFILELOADER_H
# define PHP_FASTFILELOADER_H

extern zend_module_entry fastfileloader_module_entry;
# define phpext_fastfileloader_ptr &fastfileloader_module_entry

# define PHP_FASTFILELOADER_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_FASTFILELOADER)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_FASTFILELOADER_H */
