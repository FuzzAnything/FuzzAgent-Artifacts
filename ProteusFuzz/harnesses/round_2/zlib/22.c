// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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
    if (fd < 0) return;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    gzFile file_read = gzopen("./dummy_file", "rb");
    gzFile file_write = gzopen("./dummy_file", "wb");

    if (file_read == NULL && file_write == NULL) {
        return 0;
    }

    // Fuzz gzseek and gzseek64
    if (file_read != NULL) {
        int seek_mode = Data[0] % 3; // SEEK_SET, SEEK_CUR, SEEK_END
        z_off_t offset32 = (z_off_t)(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);
        z_off64_t offset64 = (z_off64_t)(Data[5] << 56 | Data[6] << 48 | Data[7] << 40 | Data[8] << 32 | Data[9] << 24 | Data[10] << 16 | Data[11] << 8 | Data[12]);

        if (Size >= 13) {
            gzseek(file_read, offset32, seek_mode);
            gzseek64(file_read, offset64, seek_mode);
        }
    }

    // Fuzz gzoffset and gzoffset64
    if (file_read != NULL) {
        gzoffset(file_read);
        gzoffset64(file_read);
    }
    if (file_write != NULL) {
        gzoffset(file_write);
        gzoffset64(file_write);
    }

    // Fuzz gztell and gztell64
    if (file_read != NULL) {
        gztell(file_read);
        gztell64(file_read);
    }
    if (file_write != NULL) {
        gztell(file_write);
        gztell64(file_write);
    }

    // Cleanup
    if (file_read != NULL) {
        gzclose(file_read);
    }
    if (file_write != NULL) {
        gzclose(file_write);
    }

    unlink("./dummy_file");

    return 0;
}