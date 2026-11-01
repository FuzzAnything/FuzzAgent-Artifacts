// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

static int dummy_vprintf_helper(gzFile file, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = gzvprintf(file, fmt, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "w+b");
    if (fp == NULL) {
        return 0;
    }

    // Initialize gzFile for writing
    gzFile gz = gzdopen(fileno(fp), "wb");
    if (gz == NULL) {
        fclose(fp);
        return 0;
    }

    // Ensure we have enough data to work with
    if (Size < 2) {
        cleanup_gzfile(gz);
        fclose(fp);
        return 0;
    }

    // Use part of the input as a format string (ensure null termination)
    size_t format_len = Size - 1;
    if (format_len > 1024) format_len = 1024;
    
    char *format_str = (char *)malloc(format_len + 1);
    if (format_str == NULL) {
        cleanup_gzfile(gz);
        fclose(fp);
        return 0;
    }
    
    memcpy(format_str, Data, format_len);
    format_str[format_len] = '\0';

    // Use part of the input as a string for gzputs
    char *str_data = (char *)malloc(Size + 1);
    if (str_data == NULL) {
        free(format_str);
        cleanup_gzfile(gz);
        fclose(fp);
        return 0;
    }
    memcpy(str_data, Data, Size);
    str_data[Size] = '\0';

    // Test gzputc
    if (Size > 0) {
        int c = Data[0] & 0xFF;
        gzputc(gz, c);
    }

    // Test gzputs
    gzputs(gz, str_data);

    // Test gzprintf with a simple format
    // We use a safe subset of the input to avoid complex format string issues
    char safe_format[64];
    size_t safe_len = (Size < 63) ? Size : 63;
    memcpy(safe_format, Data, safe_len);
    safe_format[safe_len] = '\0';
    
    // Ensure format string is safe for printf-like functions
    // Replace potential problematic format specifiers
    for (size_t i = 0; i < safe_len; i++) {
        if (safe_format[i] == '%') {
            // Skip next char if it looks like a specifier to avoid crashes
            if (i + 1 < safe_len && (safe_format[i+1] == 's' || safe_format[i+1] == 'd' || 
                safe_format[i+1] == 'c' || safe_format[i+1] == 'x' || safe_format[i+1] == 'X')) {
                safe_format[i] = ' ';
            }
        }
    }
    
    // Test gzprintf
    gzprintf(gz, safe_format);

    // Test gzvprintf via helper
    dummy_vprintf_helper(gz, safe_format);

    // Test gzerror
    int errnum = 0;
    const char *err_msg = gzerror(gz, &errnum);
    (void)err_msg; // Suppress unused variable warning

    // Test gzseek
    if (Size > 1) {
        z_off_t offset = (z_off_t)(Data[1] & 0xFF);
        int whence = (Size > 2) ? (Data[2] % 3) : 0; // SEEK_SET, SEEK_CUR, or SEEK_END
        gzseek(gz, offset, whence);
    }

    // Cleanup
    gzclose(gz);
    free(format_str);
    free(str_data);
    fclose(fp);
    
    // Remove temporary file
    remove(filename);

    return 0;
}