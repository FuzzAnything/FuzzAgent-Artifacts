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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Open a file for writing
    const char *filename = "./dummy_file";
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    // Use first byte for gzputc if available
    if (Size > 0) {
        int c = Data[0];
        gzputc(file, c);
    }
    
    // Use remaining data for gzputs if available
    if (Size > 1) {
        // Create null-terminated string from remaining data
        size_t str_size = Size - 1;
        char *str = (char *)malloc(str_size + 1);
        if (str != NULL) {
            memcpy(str, Data + 1, str_size);
            str[str_size] = '\0';
            gzputs(file, str);
            free(str);
        }
    }
    
    // Check for errors after gzputs
    int errnum1;
    const char *error1 = gzerror(file, &errnum1);
    
    // Use some data for gzprintf if available
    if (Size > 2) {
        // Use a simple format string with the data
        size_t fmt_size = Size > 50 ? 50 : Size - 2;
        char *fmt_str = (char *)malloc(fmt_size + 1);
        if (fmt_str != NULL) {
            memcpy(fmt_str, Data + 2, fmt_size);
            fmt_str[fmt_size] = '\0';
            gzprintf(file, "Data: %s", fmt_str);
            free(fmt_str);
        }
    }
    
    // Check for errors after gzprintf
    int errnum2;
    const char *error2 = gzerror(file, &errnum2);
    
    // Try to seek to different positions
    if (Size > 0) {
        // Use first byte as offset
        z_off_t offset = Data[0];
        gzseek(file, offset, SEEK_SET);
    }
    
    // Clean up
    gzclose(file);
    
    // Remove the dummy file
    remove(filename);
    
    return 0;
}