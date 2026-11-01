// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    magic_t m = NULL;
    magic_t m_list = NULL;
    int ret;
    const char *result;
    int flags;

    // 1. Prepare environment for main magic handle
    m = magic_open(0);
    if (m == NULL) {
        return 0;
    }

    // 2. Test magic_setflags with various flags derived from input
    if (Size > 0) {
        int test_flags = (int)(Data[0] & 0xFF);
        // Combine with some standard flags to ensure variety
        test_flags |= MAGIC_MIME_TYPE;
        if (Size > 1) test_flags |= (Data[1] & 0xFF) << 8;
        
        magic_setflags(m, test_flags);
    }

    // 3. Test magic_load (requires a valid magic file, usually system default)
    // We attempt to load the default magic file. If it fails, we continue.
    magic_load(m, NULL);

    // 4. Test magic_getflags
    flags = magic_getflags(m);
    (void)flags; // Suppress unused warning

    // 5. Test magic_file with a dummy file containing fuzz input
    write_dummy_file(Data, Size);
    result = magic_file(m, "./dummy_file");
    if (result != NULL) {
        // Just to ensure we use the result and avoid compiler warnings
        (void)strlen(result);
    }

    // 6. Test magic_list with a SEPARATE handle as per hint
    m_list = magic_open(0);
    if (m_list != NULL) {
        // magic_list requires a magic file to load rules for listing
        // We try loading the default magic file again
        ret = magic_list(m_list, NULL);
        (void)ret;
        magic_close(m_list);
    }

    // 7. Cleanup
    magic_close(m);
    
    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}