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
#include <zlib.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Open a gzip file for writing
    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) {
        return 0;
    }

    // Fuzz gzwrite with the input data
    // Cast Data to voidpc and use Size as length
    int written = gzwrite(file, (voidpc)Data, (unsigned)Size);
    
    // Invoke gzerror to check for errors after write
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    // error_msg is a string, we don't need to free it, just use it if needed
    (void)error_msg;
    (void)errnum;

    // Invoke gzclose to clean up
    int close_status = gzclose(file);
    (void)close_status;

    return 0;
}