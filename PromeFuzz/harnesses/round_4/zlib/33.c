// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void test_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    gzvprintf(file, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Open file for writing
    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) return 0;
    
    // Test gzputc with first byte
    int putc_result = gzputc(file, Data[0]);
    
    // Test gzputs with a null-terminated string from input
    size_t str_len = Size > 100 ? 100 : Size - 1;
    char *str = (char *)malloc(str_len + 1);
    if (str) {
        memcpy(str, Data + 1, str_len);
        str[str_len] = '\0';
        gzputs(file, str);
        free(str);
    }
    
    // Test gzprintf with various format specifiers
    int int_val = Size > 10 ? Data[10] : 42;
    gzprintf(file, "Size: %zu, Int: %d, Char: %c", Size, int_val, Data[0]);
    
    // Test gzvprintf with variable arguments
    test_gzvprintf(file, "Vprintf test: %d %zu", int_val, Size);
    
    // Check for errors
    int errnum;
    const char *error_msg = gzerror(file, &errnum);
    
    // Close the file
    gzclose(file);
    
    // Reopen for reading to test error handling on closed/invalid file
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (read_file) {
        gzerror(read_file, &errnum);
        gzclose(read_file);
    }
    
    return 0;
}