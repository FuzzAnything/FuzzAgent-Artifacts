// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
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
#include <unistd.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create dummy file for writing
    const char *filename = "./dummy_file";
    
    // Open file for writing compressed data
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    // Write data using gzwrite
    // Ensure len fits in int (gzwrite expects unsigned int)
    unsigned int len = (Size > UINT_MAX) ? UINT_MAX : (unsigned int)Size;
    int written = gzwrite(file, (voidpc)Data, len);
    
    // Check for errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    
    // Close the file
    int close_result = gzclose(file);
    
    // Clean up dummy file
    unlink(filename);
    
    // Use the return values to avoid optimization
    (void)written;
    (void)error_msg;
    (void)errnum;
    (void)close_result;
    
    return 0;
}