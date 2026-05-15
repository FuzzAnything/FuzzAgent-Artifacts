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
#include <unistd.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fclose(fp);
    
    // Open the file for writing with gzip compression
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        unlink(filename);
        return 0;
    }
    
    // Prepare buffer for gzwrite
    // Ensure len fits in int as required by gzwrite signature
    unsigned len = (unsigned)(Size > UINT_MAX ? UINT_MAX : Size);
    voidpc buf = (voidpc)Data;
    
    // Call gzwrite
    int written = gzwrite(file, buf, len);
    
    // Call gzerror
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    
    // Call gzclose
    int close_result = gzclose(file);
    
    // Clean up the dummy file
    unlink(filename);
    
    return 0;
}