#include "php.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Global variable to hold the resource type for file handles
int le_file_resource;

// Declare the functions to be exported
PHP_FUNCTION(fastfileloader_open);
PHP_FUNCTION(fastfileloader_readline);
PHP_FUNCTION(fastfileloader_close);

// Argument info for the functions
ZEND_BEGIN_ARG_INFO(arginfo_fastfileloader_open, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fastfileloader_readline, 0)
ZEND_ARG_INFO(0, fileResource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fastfileloader_close, 0)
ZEND_ARG_INFO(0, fileResource)
ZEND_END_ARG_INFO()


PHP_MINIT_FUNCTION(myfileloader) {
        le_file_resource = zend_register_list_destructors_ex(NULL, NULL, "fastfileloader file", module_number);
        return SUCCESS;
}

// Function entries, binding PHP functions to C functions
        const zend_function_entry fastfileloader_functions[] = {
                PHP_FE(fastfileloader_open, arginfo_fastfileloader_open)
                PHP_FE(fastfileloader_readline, arginfo_fastfileloader_readline)
                PHP_FE(fastfileloader_close, arginfo_fastfileloader_close)
                PHP_FE_END
        };

// Module entry structure
        zend_module_entry fastfileloader_module_entry = {
                STANDARD_MODULE_HEADER,
                "fastfileloader",
                fastfileloader_functions,
                PHP_MINIT(myfileloader),
                NULL,
                NULL,
                NULL,
                NULL,
                NO_VERSION_YET,
                STANDARD_MODULE_PROPERTIES
        };

// Store the mmap pointer and file size together
struct mmap_obj {
    char *data;
    size_t size;
};

// Open a file and return a resource
PHP_FUNCTION(fastfileloader_open) {
        char *filename = NULL;
        size_t filename_len;
        int fd;
        struct stat fileInfo;
        char *data;

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &filename, &filename_len) == FAILURE) {
            RETURN_FALSE;
        }

        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            php_error_docref(NULL, E_WARNING, "Unable to open file: %s", filename);
            RETURN_FALSE;
        }

        if (fstat(fd, &fileInfo) == -1) {
            php_error_docref(NULL, E_WARNING, "Unable to get file size: %s", filename);
            close(fd);
            RETURN_FALSE;
        }

        data = mmap(0, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (data == MAP_FAILED) {
            php_error_docref(NULL, E_WARNING, "Unable to mmap file: %s", filename);
            close(fd);
            RETURN_FALSE;
        }

        close(fd);  // File descriptor no longer needed after mmap

        struct mmap_obj *obj = emalloc(sizeof(struct mmap_obj));
        obj->data = data;
        obj->size = fileInfo.st_size;

        RETURN_RES(zend_register_resource(obj, le_file_resource));
}

// Read a line from the file resource
PHP_FUNCTION(fastfileloader_readline) {
        zval *z_mmap_obj;
        struct mmap_obj *obj;
        static size_t offset = 0; // maintain a static offset

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &z_mmap_obj) == FAILURE) {
            RETURN_FALSE;
        }

        if ((obj = (struct mmap_obj *)zend_fetch_resource(Z_RES_P(z_mmap_obj), "fastfileloader file", le_file_resource)) == NULL) {
            RETURN_FALSE;
        }

        if (offset >= obj->size) {
            offset = 0;  // Reset offset if it reaches end
            RETVAL_FALSE; // Return FALSE when end of file is reached
            return;
        }

        char *start = obj->data + offset;
        char *end = memchr(start, '\n', obj->size - offset);
        size_t len = (end) ? (size_t)(end - start) : (obj->size - offset);

        RETVAL_STRINGL(start, len);
        offset += len + ((end) ? 1 : 0); // Move offset past the newline character
}


// Close the file resource
PHP_FUNCTION(fastfileloader_close) {
        zval *z_mmap_obj;
        struct mmap_obj *obj;

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &z_mmap_obj) == FAILURE) {
            RETURN_FALSE;
        }

        if ((obj = (struct mmap_obj *)zend_fetch_resource(Z_RES_P(z_mmap_obj), "fastfileloader file", le_file_resource)) == NULL) {
            RETURN_FALSE;
        }

        if (munmap(obj->data, obj->size) == -1) {
            php_error_docref(NULL, E_WARNING, "Error un-mmapping the file");
            RETURN_FALSE;
        }

        efree(obj); // Free the struct holding mmap details
        zend_list_close(Z_RES_P(z_mmap_obj)); // Close the resource
        RETURN_TRUE;
}

// Macros for defining entry points
ZEND_GET_MODULE(fastfileloader)
