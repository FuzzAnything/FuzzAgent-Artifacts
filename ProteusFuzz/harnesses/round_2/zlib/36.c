// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb+");
    if (f == NULL) {
        return 0;
    }

    // Write input data to the file to simulate content
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    fclose(f);

    // Reopen file for writing in gzip mode
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        remove(filename);
        return 0;
    }

    // Test 1: gzputc - write a single character
    if (Size > 0) {
        int c = Data[0] & 0xFF;
        gzputc(file, c);
    }

    // Test 2: gzprintf - format and write data
    if (Size > 1) {
        // Construct a simple format string from input to avoid crashes
        char fmt[16];
        size_t fmt_len = (Size > 15) ? 15 : Size - 1;
        memcpy(fmt, Data + 1, fmt_len);
        fmt[fmt_len] = '\0';
        // Ensure format string is safe by limiting to simple formats
        if (strchr(fmt, '%') == NULL) {
            fmt[0] = 'H'; fmt[1] = 'e'; fmt[2] = 'l'; fmt[3] = 'l'; fmt[4] = 'o'; fmt[5] = '\0';
        }
        gzprintf(file, fmt);
    }

    // Test 3: gzsetparams - change compression level and strategy
    if (Size > 2) {
        int level = (Data[1] % 10); // 0-9
        int strategy = (Data[2] % 3); // 0-2
        gzsetparams(file, level, strategy);
    }

    // Test 4: gzflush - flush pending data
    if (Size > 3) {
        int flush_mode = Data[3] % 5; // 0 to Z_FINISH (4)
        gzflush(file, flush_mode);
    }

    // Test 5: gzdirect - check if file is in transparent mode
    // Note: This is typically used on read-mode files, but we call it on write-mode to test error handling
    gzdirect(file);

    // Test 6: gzclose_w - close the file properly
    gzclose_w(file);

    // Cleanup: remove the dummy file
    remove(filename);

    return 0;
}