// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void test_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    gzvprintf(file, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create dummy file with fuzzer data
    write_dummy_file(Data, Size);
    
    // Test gzopen with various modes
    const char *modes[] = {"rb", "wb", "ab", "xb", "wb6", "wb9", "r", "w"};
    int mode_idx = Data[0] % (sizeof(modes)/sizeof(modes[0]));
    
    gzFile file = gzopen("./dummy_file", modes[mode_idx]);
    if (!file) {
        // If opening fails, try writing mode with different data
        file = gzopen("./dummy_file", "wb");
        if (!file) return 0;
    }
    
    // Extract format string from data (limited to safe characters)
    char format[32];
    size_t format_len = (Data[1] % 30) + 1;
    if (format_len > Size - 2) format_len = Size - 2;
    if (format_len > 31) format_len = 31;
    
    for (size_t i = 0; i < format_len; i++) {
        char c = Data[2 + i] % 128;
        // Only allow safe format characters
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || c == ' ' || c == '.' || 
            c == '%' || c == 'd' || c == 's' || c == 'c' || 
            c == 'x' || c == 'f') {
            format[i] = c;
        } else {
            format[i] = 'a';
        }
    }
    format[format_len] = '\0';
    
    // Extract test string from remaining data
    size_t str_offset = 2 + format_len;
    size_t str_len = Size - str_offset;
    if (str_len > 100) str_len = 100;
    
    char *test_str = NULL;
    if (str_len > 0) {
        test_str = (char*)malloc(str_len + 1);
        if (test_str) {
            memcpy(test_str, Data + str_offset, str_len);
            test_str[str_len] = '\0';
        }
    }
    
    // Test gzprintf with various inputs
    if (test_str) {
        gzprintf(file, format, test_str, (int)Size, (int)Data[0]);
    }
    
    // Test gzvprintf
    if (test_str) {
        test_gzvprintf(file, format, test_str, (int)Size, (int)Data[0]);
    }
    
    // Test gzputs
    if (test_str) {
        gzputs(file, test_str);
    }
    
    // Test gzputc with various bytes
    for (int i = 0; i < 10 && i < (int)Size; i++) {
        gzputc(file, Data[i]);
    }
    
    // Test gzerror
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    (void)error_msg; // Suppress unused warning
    
    // Clear error state if needed
    if (errnum != Z_OK) {
        gzclearerr(file);
    }
    
    // Cleanup
    if (test_str) free(test_str);
    gzclose(file);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}