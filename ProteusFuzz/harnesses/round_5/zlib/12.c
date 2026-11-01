// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzoffset at gzlib.c:490:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void write_dummy_data(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int ret;
    z_off64_t pos64;
    z_off_t pos;
    
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with fuzz input
    write_dummy_data(Data, Size);

    // Test 1: Open for reading and test all functions
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        // Test gzrewind
        ret = gzrewind(file);
        (void)ret;

        // Test gztell64
        pos64 = gztell64(file);
        (void)pos64;

        // Test gzclearerr
        gzclearerr(file);

        // Test gzseek64 with SEEK_SET
        pos64 = gzseek64(file, 0, SEEK_SET);
        (void)pos64;

        // Test gzseek64 with SEEK_CUR
        pos64 = gzseek64(file, 1, SEEK_CUR);
        (void)pos64;

        // Test gzoffset64
        pos64 = gzoffset64(file);
        (void)pos64;

        // Test gzoffset
        pos = gzoffset(file);
        (void)pos;

        gzclose(file);
        file = NULL;
    }

    // Test 2: Open for writing and test applicable functions
    file = gzopen("./dummy_file", "wb");
    if (file != NULL) {
        // Write some data to ensure valid state
        gzwrite(file, Data, Size);

        // Test gzrewind (should fail or handle gracefully in write mode)
        ret = gzrewind(file);
        (void)ret;

        // Test gztell64
        pos64 = gztell64(file);
        (void)pos64;

        // Test gzclearerr
        gzclearerr(file);

        // Test gzseek64 with SEEK_SET
        pos64 = gzseek64(file, 0, SEEK_SET);
        (void)pos64;

        // Test gzoffset64
        pos64 = gzoffset64(file);
        (void)pos64;

        // Test gzoffset
        pos = gzoffset(file);
        (void)pos;

        gzclose(file);
        file = NULL;
    }

    // Test 3: Test with NULL file handle (functions should handle gracefully)
    ret = gzrewind(NULL);
    (void)ret;

    pos64 = gztell64(NULL);
    (void)pos64;

    gzclearerr(NULL);

    pos64 = gzseek64(NULL, 0, SEEK_SET);
    (void)pos64;

    pos64 = gzoffset64(NULL);
    (void)pos64;

    pos = gzoffset(NULL);
    (void)pos;

    // Cleanup
    remove_dummy_file();

    return 0;
}