// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "zlib.h"

static int helper_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    int ret;
    
    va_start(args, format);
    ret = gzvprintf(file, format, args);
    va_end(args);
    
    return ret;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    gzFile write_file = NULL;
    gzFile read_file = NULL;
    int errnum = 0;
    const char *error_msg = NULL;
    int result = 0;
    
    /* Create a dummy file for writing */
    write_file = gzopen("./dummy_file", "wb");
    if (write_file == NULL) {
        goto cleanup;
    }
    
    /* Test gzputc with first byte if available */
    if (Size > 0) {
        result = gzputc(write_file, (int)Data[0]);
        if (result == -1) {
            error_msg = gzerror(write_file, &errnum);
        }
    }
    
    /* Test gzputs with a null-terminated string */
    if (Size > 1) {
        /* Create null-terminated string from input data */
        size_t str_len = Size < 256 ? Size : 256;
        char *str = (char *)malloc(str_len + 1);
        if (str != NULL) {
            memcpy(str, Data, str_len);
            str[str_len] = '\0';
            result = gzputs(write_file, str);
            if (result == -1) {
                error_msg = gzerror(write_file, &errnum);
            }
            free(str);
        }
    }
    
    /* Test gzprintf with simple format string */
    if (Size > 2) {
        char fmt_str[64];
        size_t fmt_len = Size < 32 ? Size : 32;
        memcpy(fmt_str, Data, fmt_len);
        fmt_str[fmt_len] = '\0';
        
        result = gzprintf(write_file, "Data: %s", fmt_str);
        if (result == -1) {
            error_msg = gzerror(write_file, &errnum);
        }
    }
    
    /* Test gzvprintf with simple format string */
    if (Size > 3) {
        char fmt_str2[64];
        size_t fmt_len2 = Size < 32 ? Size : 32;
        memcpy(fmt_str2, Data + (Size / 2), fmt_len2);
        fmt_str2[fmt_len2] = '\0';
        
        result = helper_gzvprintf(write_file, "Value: %s", fmt_str2);
        if (result != Z_OK) {
            error_msg = gzerror(write_file, &errnum);
        }
    }
    
    gzclose(write_file);
    write_file = NULL;
    
    /* Now open the same file for reading */
    read_file = gzopen("./dummy_file", "rb");
    if (read_file == NULL) {
        goto cleanup;
    }
    
    /* Test gzgetc by reading several bytes */
    for (int i = 0; i < 100; i++) {
        result = gzgetc(read_file);
        if (result == -1) {
            break;
        }
    }
    
    /* Test gzerror on read file */
    error_msg = gzerror(read_file, &errnum);
    
cleanup:
    if (write_file != NULL) {
        gzclose(write_file);
    }
    if (read_file != NULL) {
        gzclose(read_file);
    }
    
    /* Clean up dummy file */
    remove("./dummy_file");
    
    return 0;
}