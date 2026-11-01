// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Ensure we have at least some data to write to the dummy file
    write_dummy_file(Data, Size);

    gzFile file = NULL;
    int errnum;
    char buf[1024];
    int c;
    z_off_t offset;
    const char *err_msg;

    // Step 1: gzclose (on NULL to test error handling)
    gzclose(NULL);

    // Step 2: gzopen
    file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        // If open fails, try to get error info on a null pointer (should return NULL)
        gzerror(NULL, &errnum);
        return 0;
    }

    // Step 3: gzread
    int read_bytes = gzread(file, buf, sizeof(buf));
    (void)read_bytes;

    // Step 4: gzerror
    err_msg = gzerror(file, &errnum);
    (void)err_msg;

    // Step 5: gzseek
    // Safely access Data[0] and Data[1] since we checked Size >= 2
    z_off_t seek_offset = (z_off_t)(Data[0] % 100);
    int whence = (Data[1] % 3); // SEEK_SET, SEEK_CUR, SEEK_END
    offset = gzseek(file, seek_offset, whence);
    (void)offset;

    // Step 6: gztell
    offset = gztell(file);
    (void)offset;

    // Step 7: gztell (again)
    offset = gztell(file);
    (void)offset;

    // Step 8: gzgetc
    c = gzgetc(file);
    if (c != -1) {
        // Step 9: gzungetc (push back the character we just read)
        gzungetc(c, file);
    }

    // Step 10: gzgets
    char line_buf[256];
    gzgets(file, line_buf, sizeof(line_buf));

    // Step 11: gzerror
    err_msg = gzerror(file, &errnum);
    (void)err_msg;

    // Step 12: gzclose
    gzclose(file);

    return 0;
}