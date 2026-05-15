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
#include <unistd.h>

static gzFile open_dummy_file(const char *mode) {
    const char *filename = "./dummy_file";
    return gzopen(filename, mode);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    gzFile file = open_dummy_file("wb");
    if (file == NULL) {
        return 0;
    }

    unsigned len = (unsigned)(Size > UINT_MAX ? UINT_MAX : Size);
    int written = gzwrite(file, (voidpc)Data, len);

    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    (void)error_msg;

    int close_ret = gzclose(file);
    (void)close_ret;
    (void)written;

    unlink("./dummy_file");
    return 0;
}