// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Open a dummy file for writing
    const char *filename = "./dummy_file";
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    // If we have at least 1 byte of data, test gzputc
    if (Size > 0) {
        int c = Data[0] & 0xFF;
        gzputc(file, c);
    }
    
    // If we have at least 2 bytes, test gzputs with a null-terminated string
    if (Size > 1) {
        // Create a null-terminated string from the input data
        size_t str_len = Size - 1;
        if (str_len > 1024) str_len = 1024; // Limit string length
        char *str = (char *)malloc(str_len + 1);
        if (str != NULL) {
            memcpy(str, Data + 1, str_len);
            str[str_len] = '\0';
            gzputs(file, str);
            free(str);
        }
    }
    
    // Test gzprintf with a simple format string
    if (Size > 0) {
        int value = Data[0] & 0xFF;
        gzprintf(file, "Value: %d\n", value);
    }
    
    // Test gzwrite with the input data
    if (Size > 0) {
        // Ensure len fits in unsigned int
        unsigned len = (unsigned)Size;
        if (len > 65535) len = 65535; // Reasonable limit
        gzwrite(file, (voidpc)Data, len);
    }
    
    // Close the file
    gzclose(file);
    
    // Remove the dummy file to avoid accumulation
    remove(filename);
    
    return 0;
}