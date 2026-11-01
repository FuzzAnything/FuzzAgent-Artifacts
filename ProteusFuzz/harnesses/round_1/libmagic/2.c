// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to dummy file for file-based operations
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Test magic_open with various flags
    int flags = MAGIC_NONE;
    if (Size > 0) flags = Data[0] & 0xFF;
    
    magic_t mh = magic_open(flags);
    if (mh == NULL) {
        return 0;
    }

    // Test magic_check (requires separate handle per hint)
    {
        magic_t mh_check = magic_open(MAGIC_NONE);
        if (mh_check != NULL) {
            magic_check(mh_check, "./dummy_file");
            magic_close(mh_check);
        }
    }

    // Test magic_compile
    magic_compile(mh, "./dummy_file");

    // Test magic_load
    magic_load(mh, "./dummy_file");

    // Test magic_file
    const char *result = magic_file(mh, "./dummy_file");
    (void)result; // Suppress unused warning

    // Test magic_list (requires separate handle per hint)
    {
        magic_t mh_list = magic_open(MAGIC_NONE);
        if (mh_list != NULL) {
            magic_list(mh_list, "./dummy_file");
            magic_close(mh_list);
        }
    }

    // Cleanup
    magic_close(mh);

    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}