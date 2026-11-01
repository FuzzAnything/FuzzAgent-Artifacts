// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int create_dummy_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            unlink(filename);
            return -1;
        }
    }
    close(fd);
    return 0;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    magic_t m;
    magic_t m_list;
    const char *result;
    int ret;

    // Test magic_open with various flags
    int flags[] = {MAGIC_NONE, MAGIC_MIME_TYPE, MAGIC_MIME_ENCODING, MAGIC_SYMLINK, MAGIC_ERROR, MAGIC_COMPRESS, MAGIC_RAW};
    int num_flags = sizeof(flags) / sizeof(flags[0]);

    for (int i = 0; i < num_flags; i++) {
        m = magic_open(flags[i]);
        if (m == NULL) {
            continue;
        }

        // Test magic_load with a dummy path (likely to fail if file doesn't exist, which is expected)
        ret = magic_load(m, NULL);
        (void)ret; // Ignore return, just exercising the path

        // Test magic_buffer
        result = magic_buffer(m, Data, Size);
        (void)result; // Ignore result, just exercising the path

        // Test magic_error
        const char *err = magic_error(m);
        (void)err;

        // Test magic_file with a dummy file
        if (Size > 0 && create_dummy_file(Data, Size) == 0) {
            result = magic_file(m, "./dummy_file");
            (void)result;
            cleanup_dummy_file();
        }

        magic_close(m);
    }

    // Test magic_list with a separate handle as per hint
    for (int i = 0; i < num_flags; i++) {
        m_list = magic_open(flags[i]);
        if (m_list == NULL) {
            continue;
        }

        // magic_list requires a filename, passing NULL or invalid path to test error handling
        ret = magic_list(m_list, NULL);
        (void)ret;

        // Also try with a dummy filename that likely doesn't exist
        ret = magic_list(m_list, "./nonexistent_magic_file");
        (void)ret;

        magic_close(m_list);
    }

    return 0;
}