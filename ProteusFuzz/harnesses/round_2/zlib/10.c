// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a temporary file and open it for writing
    const char *filename = "./dummy_file";
    
    // Open gzFile for writing
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    // Prepare buffer from input data
    unsigned char *buffer = (unsigned char *)malloc(Size);
    if (buffer == NULL) {
        gzclose(file);
        return 0;
    }
    memcpy(buffer, Data, Size);

    // Invoke target functions in order: gzwrite -> gzerror -> gzclose

    // 1. gzwrite
    int written = gzwrite(file, buffer, Size);
    (void)written; // Suppress unused variable warning

    // 2. gzerror
    int errnum = 0;
    const char *errmsg = gzerror(file, &errnum);
    (void)errmsg; // Suppress unused variable warning

    // 3. gzclose
    int close_status = gzclose(file);
    (void)close_status; // Suppress unused variable warning

    // Cleanup
    free(buffer);
    
    // Remove the dummy file to keep environment clean
    remove(filename);

    return 0;
}