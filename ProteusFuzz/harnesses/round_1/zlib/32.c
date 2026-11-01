// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <fcntl.h>
#include <unistd.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with fuzz input for file-based operations
    create_dummy_file(Data, Size);

    gzFile file = NULL;
    int fd = -1;
    unsigned char buf[256];

    // Test 1: gzopen64 with various modes derived from input
    const char *mode64;
    if (Size > 0) {
        int val = Data[0] % 3;
        if (val == 0) mode64 = "rb";
        else if (val == 1) mode64 = "wb";
        else mode64 = "ab";
    } else {
        mode64 = "rb";
    }

    file = gzopen64("./dummy_file", mode64);
    if (file) {
        if (mode64[0] == 'r') {
            gzfread(buf, 1, sizeof(buf), file);
        } else {
            gzfwrite(buf, 1, sizeof(buf), file);
        }
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test 2: gzopen with various modes
    const char *mode;
    if (Size > 1) {
        int val = Data[1] % 3;
        if (val == 0) mode = "rb";
        else if (val == 1) mode = "wb";
        else mode = "ab";
    } else {
        mode = "rb";
    }

    file = gzopen("./dummy_file", mode);
    if (file) {
        if (mode[0] == 'r') {
            gzfread(buf, 1, sizeof(buf), file);
        } else {
            gzfwrite(buf, 1, sizeof(buf), file);
        }
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test 3: gzdopen with an existing file descriptor
    fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        file = gzdopen(fd, "rb");
        if (file) {
            gzfread(buf, 1, sizeof(buf), file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
    }

    // Test 4: gzdopen with write mode
    fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        file = gzdopen(fd, "wb");
        if (file) {
            gzfwrite(buf, 1, sizeof(buf), file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
    }

    // Test 5: Edge cases with NULL file handle
    gzclearerr(NULL);

    // Test 6: gzfread/gzfwrite with NULL file
    unsigned char test_buf[10];
    gzfread(test_buf, 1, 10, NULL);
    gzfwrite(test_buf, 1, 10, NULL);

    // Test 7: gzfread/gzfwrite with zero size or count on valid file if available
    // Re-open for this specific test to ensure valid state
    file = gzopen("./dummy_file", "rb");
    if (file) {
        gzfread(test_buf, 0, 10, file);
        gzfread(test_buf, 1, 0, file);
        gzclose(file);
        file = NULL;
    }

    file = gzopen("./dummy_file", "wb");
    if (file) {
        gzfwrite(test_buf, 0, 10, file);
        gzfwrite(test_buf, 1, 0, file);
        gzclose(file);
        file = NULL;
    }

    return 0;
}