// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Fuzz magic_setflags
    int flags = 0;
    if (Size > 0) flags = Data[0];
    magic_setflags(cookie, flags);
    if (Size > 1) magic_setflags(cookie, Data[1]);

    // Fuzz magic_setparam
    // Note: magic_setparam expects a pointer to a size_t, but the value passed 
    // in the fuzz input is just a byte. We cast it to size_t for variety.
    size_t param_val = 0;
    
    // Try setting various parameters if available in the library version
    // We use a loop to try different parameter IDs if we knew them, 
    // but since constants might be missing or version-dependent, 
    // we rely on the fact that invalid params are ignored.
    // We will try a range of likely parameter IDs based on common libmagic versions.
    // Common IDs: 0 to 10 usually cover the main ones.
    for (int i = 0; i < 10 && Size > (size_t)(i + 2); i++) {
        param_val = (size_t)Data[i + 2];
        magic_setparam(cookie, i, &param_val);
    }

    // Fuzz magic_getparam
    size_t retrieved_val;
    for (int i = 0; i < 10 && Size > (size_t)(i + 2); i++) {
        magic_getparam(cookie, i, &retrieved_val);
    }

    // Write dummy file for magic_compile
    write_dummy_file(Data, Size);
    
    // Fuzz magic_compile
    magic_compile(cookie, "./dummy_file");
    magic_compile(cookie, NULL);

    // Fuzz magic_load_buffers
    void *bufs[2];
    size_t sizes[2];
    
    // Use input data as buffers
    if (Size > 0) {
        bufs[0] = (void *)Data;
        sizes[0] = Size;
        magic_load_buffers(cookie, bufs, sizes, 1);
    }
    
    if (Size > 1) {
        bufs[0] = (void *)Data;
        bufs[1] = (void *)(Data + 1);
        sizes[0] = Size;
        sizes[1] = Size - 1;
        magic_load_buffers(cookie, bufs, sizes, 2);
    }

    // Cleanup
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}