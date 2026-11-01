// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file to create a valid gzip stream context
    write_dummy_file(Data, Size);

    gzFile file = NULL;
    z_off_t offset32;
    z_off64_t offset64;
    int seek_mode;
    z_off_t seek_offset32;
    z_off64_t seek_offset64;

    // Determine seek mode and offsets from input data to diversify states
    seek_mode = (Size > 0) ? (Data[0] % 4) : 0;
    if (seek_mode > 2) seek_mode = 0; // Ensure valid SEEK_SET, SEEK_CUR, or SEEK_END

    if (Size > 1) {
        seek_offset32 = (z_off_t)(Data[1] * 256 + (Size > 2 ? Data[2] : 0));
    } else {
        seek_offset32 = 0;
    }

    if (Size > 3) {
        seek_offset64 = ((z_off64_t)Data[3] << 24) | ((z_off64_t)Data[4] << 16) | 
                        ((z_off64_t)Data[5] << 8) | (z_off64_t)Data[6];
    } else {
        seek_offset64 = 0;
    }

    // Try opening in read mode
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        // Test gztell
        gztell(file);
        // Test gztell64
        gztell64(file);
        // Test gzoffset
        gzoffset(file);
        // Test gzoffset64
        gzoffset64(file);
        // Test gzseek
        gzseek(file, seek_offset32, seek_mode);
        // Test gzseek64
        gzseek64(file, seek_offset64, seek_mode);
        
        // Perform a read to advance stream state
        char buf[1024];
        gzread(file, buf, sizeof(buf));

        // Re-test position functions after read
        gztell(file);
        gztell64(file);
        gzoffset(file);
        gzoffset64(file);
        gzseek(file, seek_offset32, seek_mode);
        gzseek64(file, seek_offset64, seek_mode);

        gzclose(file);
    }

    // Try opening in write mode to test write-only constraints
    file = gzopen("./dummy_file", "wb");
    if (file != NULL) {
        // Write some data to ensure valid state
        const char *test_data = "Fuzzing test data for zlib";
        gzwrite(file, test_data, strlen(test_data));
        
        // Test position functions in write mode
        gztell(file);
        gztell64(file);
        gzoffset(file);
        gzoffset64(file);
        
        // Test seek in write mode (may fail or behave differently)
        gzseek(file, seek_offset32, seek_mode);
        gzseek64(file, seek_offset64, seek_mode);

        gzclose(file);
    }

    // Try opening in append mode
    file = gzopen("./dummy_file", "ab");
    if (file != NULL) {
        gztell(file);
        gztell64(file);
        gzoffset(file);
        gzoffset64(file);
        gzseek(file, seek_offset32, seek_mode);
        gzseek64(file, seek_offset64, seek_mode);
        gzclose(file);
    }

    // Test with invalid file handle (NULL)
    gztell(NULL);
    gztell64(NULL);
    gzoffset(NULL);
    gzoffset64(NULL);
    gzseek(NULL, seek_offset32, seek_mode);
    gzseek64(NULL, seek_offset64, seek_mode);

    // Test with invalid seek modes
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        gzseek(file, seek_offset32, 99); // Invalid mode
        gzseek64(file, seek_offset64, 99); // Invalid mode
        gzclose(file);
    }

    return 0;
}