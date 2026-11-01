// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    
    // Open gzip file for writing
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    // Extract parameters from input data with bounds checking
    int flush_mode = 0;
    int level = 0;
    int strategy = 0;

    if (Size > 0) {
        flush_mode = Data[0] % 10; // 0 to 9, valid range is 0-Z_FINISH (Z_FINISH is 4)
        if (flush_mode > Z_FINISH) flush_mode = Z_FINISH;
    }

    if (Size > 1) {
        level = Data[1] % 10; // 0 to 9
    }

    if (Size > 2) {
        strategy = Data[2] % 6; // 0 to 5 (Z_DEFAULT_STRATEGY to Z_FIXED)
    }

    // Prepare a string for gzputs and gzprintf
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf == NULL) {
        gzclose_w(file);
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    // Test gzputc
    for (size_t i = 0; i < Size && i < 1000; i++) {
        int c = (int)Data[i];
        gzputc(file, c);
    }

    // Test gzputs
    gzputs(file, str_buf);

    // Test gzprintf with various format specifiers
    // Ensure we have enough data before accessing indices 3, 4, 5
    if (Size > 5) {
        gzprintf(file, "Int: %d, Char: %c, Hex: %x\n", 
                 (int)Data[3], (char)Data[4], (unsigned int)Data[5]);
        gzprintf(file, "String: %s\n", str_buf);
    } else if (Size > 0) {
        // Fallback for smaller inputs
        gzprintf(file, "Int: %d\n", (int)Data[0]);
    }

    // Test gzsetparams
    gzsetparams(file, level, strategy);

    // Test gzflush
    gzflush(file, flush_mode);

    // Write more data after changing params
    for (size_t i = 0; i < Size && i < 500; i++) {
        gzputc(file, (int)Data[i]);
    }

    // Final flush and close
    gzflush(file, Z_FINISH);
    gzclose_w(file);

    free(str_buf);

    // Clean up the dummy file
    remove(filename);

    return 0;
}