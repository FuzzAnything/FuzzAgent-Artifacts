// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file to simulate a gzip stream
    write_dummy_file(Data, Size);

    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        return 0;
    }

    // Test gzbuffer: must be called immediately after gzopen
    // Try various buffer sizes including edge cases
    unsigned buffer_sizes[] = {0, 1, 8, 16, 32, 1024, 65536, (unsigned)-1};
    for (int i = 0; i < 7; i++) {
        // Reopen for each buffer test since gzbuffer can only be called once
        gzclose(file);
        file = gzopen("./dummy_file", "rb");
        if (!file) break;
        gzbuffer(file, buffer_sizes[i]);
    }
    gzclose(file);
    file = gzopen("./dummy_file", "rb");
    if (!file) return 0;

    // Buffer for reading
    char buf[256];
    memset(buf, 0, sizeof(buf));

    // Test gzread
    for (int i = 0; i < 5; i++) {
        unsigned len = (Size > 0) ? (Size % 100) + 1 : 1;
        if (len > sizeof(buf)) len = sizeof(buf);
        int ret = gzread(file, buf, len);
        if (ret == -1) {
            int errnum;
            gzerror(file, &errnum);
        }
    }

    // Test gzgetc
    for (int i = 0; i < 10; i++) {
        int c = gzgetc(file);
        if (c == -1) {
            int errnum;
            gzerror(file, &errnum);
        }
    }

    // Test gzgets
    for (int i = 0; i < 5; i++) {
        int len = (Size > 0) ? (Size % 256) + 1 : 1;
        if (len > sizeof(buf)) len = sizeof(buf);
        char *ret = gzgets(file, buf, len);
        if (ret == NULL) {
            int errnum;
            gzerror(file, &errnum);
        }
    }

    // Test gzungetc: push back various characters
    for (int i = 0; i < 10; i++) {
        int c = (i < 256) ? i : -1;
        if (c == -1) continue; // Skip EOF
        int ret = gzungetc(c, file);
        if (ret == -1) {
            int errnum;
            gzerror(file, &errnum);
        }
    }

    // Additional reads to consume pushed back data
    for (int i = 0; i < 10; i++) {
        gzgetc(file);
    }

    // Final error check
    int errnum;
    const char *err_msg = gzerror(file, &errnum);
    (void)err_msg; // Suppress unused warning

    gzclose(file);
    return 0;
}