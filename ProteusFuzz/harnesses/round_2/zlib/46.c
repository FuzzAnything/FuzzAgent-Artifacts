// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static int test_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = gzvprintf(file, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations if needed
    char *buffer = (char *)malloc(Size + 1);
    if (!buffer) {
        return 0;
    }
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    gzFile file = NULL;
    int fd = -1;
    int errnum = 0;
    int ret = 0;

    // Test 1: gzopen with dummy file
    write_dummy_file(Data, Size);
    file = gzopen("./dummy_file", "wb");
    if (file) {
        // Test gzputs
        ret = gzputs(file, buffer);
        (void)ret;

        // Test gzprintf
        ret = gzprintf(file, "Data: %s", buffer);
        (void)ret;

        // Test gzvprintf via helper
        ret = test_gzvprintf(file, "VData: %s", buffer);
        (void)ret;

        gzclose(file);
        file = NULL;
    }

    // Test 2: gzdopen with a temporary file descriptor
    fd = open("./dummy_file", O_RDWR);
    if (fd >= 0) {
        file = gzdopen(fd, "wb");
        if (file) {
            ret = gzputs(file, buffer);
            (void)ret;

            ret = gzprintf(file, "FD Data: %s", buffer);
            (void)ret;

            gzclose(file);
            file = NULL;
        }
        close(fd);
        fd = -1;
    }

    // Test 3: Error handling with invalid file
    file = gzopen("./dummy_file", "rb");
    if (file) {
        // Intentionally try to write to a read-only file to trigger error
        ret = gzputs(file, buffer);
        (void)ret;

        // Check error
        const char *err = gzerror(file, &errnum);
        (void)err;

        gzclose(file);
        file = NULL;
    }

    // Test 4: Error handling with NULL file
    const char *err = gzerror(NULL, &errnum);
    (void)err;

    // Test 5: gzdopen with invalid fd
    file = gzdopen(-1, "wb");
    (void)file;

    // Cleanup
    free(buffer);
    return 0;
}