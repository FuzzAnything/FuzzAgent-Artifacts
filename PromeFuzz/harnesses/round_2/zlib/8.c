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
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    static const char *filename = "./dummy_file";
    
    // Open a gzFile for writing
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    // Write data in chunks if Size is larger than unsigned int max
    size_t remaining = Size;
    const uint8_t *current = Data;
    int write_result = 0;
    
    while (remaining > 0) {
        unsigned chunk_size;
        if (remaining > 0xFFFFFFFFU) {
            chunk_size = 0xFFFFFFFFU;
        } else {
            chunk_size = (unsigned)remaining;
        }
        
        write_result = gzwrite(file, (voidpc)current, chunk_size);
        if (write_result == 0) {
            // Write error occurred
            break;
        }
        
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    // Check for errors after writing
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    (void)error_msg; // Suppress unused variable warning
    
    // Close the file
    int close_result = gzclose(file);
    (void)close_result; // Suppress unused variable warning
    
    return 0;
}