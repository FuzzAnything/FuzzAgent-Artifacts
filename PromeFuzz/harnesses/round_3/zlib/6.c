// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create dummy file for gzopen
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzer data to the dummy file
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Open the file with gzopen for writing
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        unlink(filename);
        return 0;
    }
    
    // gzputc - write a single byte
    int byte_to_write = (Size > 0) ? Data[0] : 'A';
    gzputc(file, byte_to_write);
    
    // gzputs - write a string
    const char *test_string = "fuzz test string";
    gzputs(file, test_string);
    
    // First gzerror call
    int errnum1;
    const char *error_msg1 = gzerror(file, &errnum1);
    
    // gzprintf - write formatted data
    // Create a safe format string that persists in scope
    char safe_format[256];
    const char *format_str;
    
    if (Size > 1) {
        // Copy fuzzer data to safe_format
        size_t format_len = (Size - 1 < 255) ? Size - 1 : 255;
        memcpy(safe_format, Data + 1, format_len);
        safe_format[format_len] = '\0';
        
        // Ensure format string doesn't contain problematic format specifiers
        for (size_t i = 0; i < format_len; i++) {
            if (safe_format[i] == '%') {
                safe_format[i] = '#';
            }
        }
        format_str = safe_format;
    } else {
        format_str = "Test data: %d";
    }
    
    int value_to_print = (Size > 2) ? Data[2] : 42;
    gzprintf(file, format_str, value_to_print);
    
    // Second gzerror call
    int errnum2;
    const char *error_msg2 = gzerror(file, &errnum2);
    
    // gzseek - set file position
    z_off_t offset = (Size > 3) ? (z_off_t)Data[3] : 0;
    int whence = (Size > 4) ? (Data[4] % 3) : SEEK_SET; // SEEK_SET=0, SEEK_CUR=1, SEEK_END=2
    gzseek(file, offset, whence);
    
    // Cleanup
    gzclose(file);
    unlink(filename);
    
    return 0;
}