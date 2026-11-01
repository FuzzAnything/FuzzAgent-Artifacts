// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    magic_t cookie_list = NULL;
    const char *result;
    int ret;
    const char *err_msg;
    int err_code;

    // 1. Prepare environment for magic_open
    // Test various flag combinations to explore states
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0x07; // Use first byte to determine flags (MAGIC_* constants)
    }

    cookie = magic_open(flags);
    if (cookie == NULL) {
        // Handle allocation failure gracefully
        return 0;
    }

    // 2. Test magic_list with a separate handle as per hint
    int list_flags = 0;
    if (Size > 1) {
        list_flags = Data[1] & 0x07;
    }
    cookie_list = magic_open(list_flags);
    if (cookie_list != NULL) {
        // magic_list requires a magic file. We try to load a non-existent or dummy file.
        // This is expected to fail often, which is good for fuzzing error paths.
        ret = magic_list(cookie_list, "./dummy_file");
        if (ret == -1) {
            err_code = magic_errno(cookie_list);
            err_msg = magic_error(cookie_list);
            (void)err_code;
            (void)err_msg;
        }
        magic_close(cookie_list);
    }

    // 3. Prepare dummy file for magic_file
    write_dummy_file(Data, Size);

    // 4. Invoke magic_file
    result = magic_file(cookie, "./dummy_file");
    if (result == NULL) {
        err_code = magic_errno(cookie);
        err_msg = magic_error(cookie);
        (void)err_code;
        (void)err_msg;
    }

    // 5. Test magic_load_buffers
    // We need to create buffers to feed.
    // Strategy: Use the input data as the buffer content.
    // We create a small array of buffers to test the array logic.
    void *buffers[4];
    size_t sizes[4];
    size_t count = 0;

    if (Size > 0) {
        buffers[0] = (void *)Data;
        sizes[0] = Size;
        count = 1;

        // Try to split data into multiple buffers if size is large enough
        if (Size > 8) {
            buffers[1] = (void *)(Data + 4);
            sizes[1] = Size - 4;
            count = 2;
        }
        if (Size > 16) {
            buffers[2] = (void *)(Data + 8);
            sizes[2] = Size - 8;
            count = 3;
        }
        if (Size > 24) {
            buffers[3] = (void *)(Data + 12);
            sizes[3] = Size - 12;
            count = 4;
        }
    }

    // Invoke magic_load_buffers
    // Note: magic_load_buffers expects the buffers to be loaded into the magic database.
    // This function is specific to loading magic rules from buffers, not analyzing data buffers.
    // However, the prompt says "feeds multiple memory buffers to a libmagic database".
    // We pass the fuzz input as if it were a magic file content.
    ret = magic_load_buffers(cookie, buffers, sizes, count);
    if (ret == -1) {
        err_code = magic_errno(cookie);
        err_msg = magic_error(cookie);
        (void)err_code;
        (void)err_msg;
    }

    // 6. Cleanup
    magic_close(cookie);

    return 0;
}