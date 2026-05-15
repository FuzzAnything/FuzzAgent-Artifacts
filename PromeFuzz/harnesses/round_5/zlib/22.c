// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "zlib.h"

static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return -1;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file with fuzzer data
    if (create_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Test gzopen
    gzFile file1 = gzopen("./dummy_file", "rb");
    if (file1) {
        // Test gzgets
        char buf[256];
        int len = (int)(Size % 256);
        if (len == 0) len = 1;
        char *result = gzgets(file1, buf, len);
        (void)result; // Suppress unused warning
        
        // Test gzungetc
        if (Size > 1) {
            int c = (int)(Data[0] & 0xFF);
            if (c != EOF) {
                gzungetc(c, file1);
            }
        }
        gzclose(file1);
    }

    // Test gzopen64
    gzFile file2 = gzopen64("./dummy_file", "rb");
    if (file2) {
        gzclose(file2);
    }

    // Test gzdopen
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, "rb");
        if (file3) {
            gzclose(file3);
        } else {
            close(fd);
        }
    }

    // Test gzprintf with a new file
    gzFile file4 = gzopen("./dummy_file_out", "wb");
    if (file4) {
        // Use a safe format string
        int written = gzprintf(file4, "Data size: %zu\n", Size);
        (void)written; // Suppress unused warning
        
        // Write some of the fuzzer data
        if (Size > 0) {
            size_t write_size = Size > 100 ? 100 : Size;
            for (size_t i = 0; i < write_size; i++) {
                gzprintf(file4, "%c", Data[i]);
            }
        }
        gzclose(file4);
    }

    // Clean up
    unlink("./dummy_file");
    unlink("./dummy_file_out");
    
    return 0;
}