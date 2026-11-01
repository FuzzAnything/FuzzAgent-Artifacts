// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <zlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    z_off_t offset32;
    z_off64_t offset64;
    z_off64_t tell64;
    int eof;
    z_off64_t seek_res;
    uint8_t buffer[1024];

    if (Size < 2) {
        return 0;
    }

    // Write data to dummy file
    write_dummy_file(Data, Size);

    // Try opening in read mode
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        // Test gzoffset
        offset32 = gzoffset(file);
        (void)offset32;

        // Test gzoffset64
        offset64 = gzoffset64(file);
        (void)offset64;

        // Test gztell64
        tell64 = gztell64(file);
        (void)tell64;

        // Test gzeof
        eof = gzeof(file);
        (void)eof;

        // Attempt to read some data to change state
        if (Size > 0) {
            size_t to_read = (Size < sizeof(buffer)) ? Size : sizeof(buffer);
            gzread(file, buffer, to_read);
        }

        // Test gzeof after read
        eof = gzeof(file);
        (void)eof;

        // Test gzseek64 with various offsets
        // SEEK_SET
        seek_res = gzseek64(file, 0, SEEK_SET);
        (void)seek_res;

        // SEEK_CUR with small offset
        seek_res = gzseek64(file, 1, SEEK_CUR);
        (void)seek_res;

        // SEEK_CUR with negative offset
        seek_res = gzseek64(file, -1, SEEK_CUR);
        (void)seek_res;

        // Test gzclearerr
        gzclearerr(file);

        gzclose(file);
        file = NULL;
    }

    // Try opening in write mode
    file = gzopen("./dummy_file", "wb");
    if (file != NULL) {
        // Write some data
        if (Size > 0) {
            gzwrite(file, Data, Size);
        }

        // Test gztell64 in write mode
        tell64 = gztell64(file);
        (void)tell64;

        // Test gzoffset64 in write mode
        offset64 = gzoffset64(file);
        (void)offset64;

        // Test gzoffset in write mode
        offset32 = gzoffset(file);
        (void)offset32;

        // Test gzclearerr in write mode
        gzclearerr(file);

        // Test gzeof in write mode (should return 0)
        eof = gzeof(file);
        (void)eof;

        gzclose(file);
        file = NULL;
    }

    // Try opening in append mode
    file = gzopen("./dummy_file", "ab");
    if (file != NULL) {
        // Write some data
        if (Size > 0) {
            gzwrite(file, Data, Size);
        }

        // Test gztell64
        tell64 = gztell64(file);
        (void)tell64;

        gzclose(file);
        file = NULL;
    }

    // Test with NULL file handle
    offset32 = gzoffset(NULL);
    (void)offset32;
    offset64 = gzoffset64(NULL);
    (void)offset64;
    tell64 = gztell64(NULL);
    (void)tell64;
    eof = gzeof(NULL);
    (void)eof;
    gzclearerr(NULL);
    seek_res = gzseek64(NULL, 0, SEEK_SET);
    (void)seek_res;

    return 0;
}