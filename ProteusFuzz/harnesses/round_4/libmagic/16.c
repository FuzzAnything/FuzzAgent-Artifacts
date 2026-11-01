// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    const char *result = NULL;
    int fd = -1;
    int ret;
    size_t param_val;
    
    // Initialize magic handle
    cookie = magic_open(MAGIC_MIME_TYPE | MAGIC_SYMLINK);
    if (cookie == NULL) {
        return 0;
    }

    // Set flags to explore different behaviors
    magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    magic_setflags(cookie, MAGIC_NO_CHECK_COMPRESS);
    magic_setflags(cookie, MAGIC_NO_CHECK_SOFT);

    // Configure parameters to test limits
    size_t indir_max = 100;
    size_t name_max = 200;
    size_t elf_shnum_max = 50000;
    size_t regex_max = 10000;
    
    magic_setparam(cookie, MAGIC_PARAM_INDIR_MAX, &indir_max);
    magic_setparam(cookie, MAGIC_PARAM_NAME_MAX, &name_max);
    magic_setparam(cookie, MAGIC_PARAM_ELF_SHNUM_MAX, &elf_shnum_max);
    magic_setparam(cookie, MAGIC_PARAM_REGEX_MAX, &regex_max);

    // Retrieve parameters to ensure they were set
    magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_val);
    magic_getparam(cookie, MAGIC_PARAM_NAME_MAX, &param_val);
    magic_getparam(cookie, MAGIC_PARAM_ELF_SHNUM_MAX, &param_val);
    magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);

    // Load buffers if data is available
    if (Size > 0) {
        void *bufs[1];
        size_t sizes[1];
        bufs[0] = (void *)Data;
        sizes[0] = Size;
        magic_load_buffers(cookie, bufs, sizes, 1);
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        if (result != NULL) {
            // Just consume the result to avoid unused variable warnings
            (void)strlen(result);
        }
    }

    // Test magic_descriptor with a file descriptor
    if (Size > 0) {
        write_dummy_file(Data, Size);
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            result = magic_descriptor(cookie, fd);
            if (result != NULL) {
                (void)strlen(result);
            }
            close(fd);
        }
    }

    // Cleanup
    magic_close(cookie);
    return 0;
}