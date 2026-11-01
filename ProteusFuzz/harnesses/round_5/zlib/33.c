// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    // Write the fuzzed data to the dummy file
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) return 0;

    // Open the file in read mode
    gzFile file = gzopen(filename, "rb");
    if (!file) return 0;

    // Allocate buffer for gzread
    // Ensure len fits in int as per documentation
    unsigned len = (Size > 256) ? 256 : Size;
    if (len > (unsigned)INT_MAX) len = (unsigned)INT_MAX;
    unsigned char *buf = (unsigned char *)malloc(len);
    if (!buf) {
        gzclose_r(file);
        return 0;
    }

    // Fuzz gzread
    int read_bytes = gzread(file, buf, len);
    (void)read_bytes; // Ignore return for fuzzing, just exercise path

    // Fuzz gzgetc
    int c = gzgetc(file);
    (void)c;

    // Fuzz gzgetc_
    int c_ = gzgetc_(file);
    (void)c_;

    // Fuzz gzeof
    int eof = gzeof(file);
    (void)eof;

    // Fuzz gzungetc with various characters
    if (c != -1) {
        gzungetc(c, file);
    }
    // Try pushing back EOF character (-1) which should fail or behave specifically
    gzungetc(-1, file);
    // Push back a random byte from input if available
    if (Size > 0) {
        gzungetc(Data[0], file);
    }

    // Re-check EOF after operations
    eof = gzeof(file);
    (void)eof;

    // Cleanup
    free(buf);
    gzclose_r(file);
    
    // Clean up dummy file
    remove(filename);

    return 0;
}