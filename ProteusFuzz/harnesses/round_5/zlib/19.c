// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
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

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }
    close(fd);

    // Test reading
    gzFile read_file = gzopen(filename, "rb");
    if (read_file != NULL) {
        // Fuzz gztell
        z_off_t tell_res = gztell(read_file);
        (void)tell_res;

        // Fuzz gztell64
        z_off64_t tell64_res = gztell64(read_file);
        (void)tell64_res;

        // Fuzz gzoffset
        z_off_t offset_res = gzoffset(read_file);
        (void)offset_res;

        // Fuzz gzoffset64
        z_off64_t offset64_res = gzoffset64(read_file);
        (void)offset64_res;

        // Fuzz gzclearerr
        gzclearerr(read_file);

        // Fuzz gzseek64 with various offsets derived from input
        if (Size >= 8) {
            z_off64_t seek_offset = 0;
            memcpy(&seek_offset, Data, 8);
            int whence = (Data[8 % Size]) % 3; // SEEK_SET, SEEK_CUR, SEEK_END
            z_off64_t seek_res = gzseek64(read_file, seek_offset, whence);
            (void)seek_res;
        }

        // Read some data to change state
        unsigned char buf[256];
        int read_len = gzread(read_file, buf, sizeof(buf));
        (void)read_len;

        // Call functions again after reading
        tell_res = gztell(read_file);
        tell64_res = gztell64(read_file);
        offset_res = gzoffset(read_file);
        offset64_res = gzoffset64(read_file);
        gzclearerr(read_file);

        cleanup_gzfile(read_file);
    }

    // Test writing
    gzFile write_file = gzopen(filename, "wb");
    if (write_file != NULL) {
        // Write some data
        gzwrite(write_file, Data, Size);

        // Fuzz gztell
        z_off_t tell_res = gztell(write_file);
        (void)tell_res;

        // Fuzz gztell64
        z_off64_t tell64_res = gztell64(write_file);
        (void)tell64_res;

        // Fuzz gzoffset
        z_off_t offset_res = gzoffset(write_file);
        (void)offset_res;

        // Fuzz gzoffset64
        z_off64_t offset64_res = gzoffset64(write_file);
        (void)offset64_res;

        // Fuzz gzclearerr
        gzclearerr(write_file);

        // Fuzz gzseek64 (write mode usually only supports SEEK_SET to 0 or current)
        if (Size >= 8) {
            z_off64_t seek_offset = 0;
            memcpy(&seek_offset, Data, 8);
            int whence = (Data[8 % Size]) % 2; // SEEK_SET, SEEK_CUR
            z_off64_t seek_res = gzseek64(write_file, seek_offset, whence);
            (void)seek_res;
        }

        cleanup_gzfile(write_file);
    }

    // Cleanup file
    unlink(filename);

    return 0;
}