// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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
#include <stdarg.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have at least a small amount of data for string operations
    // If data is too short, pad it or skip complex string ops
    const char *filename = "./dummy_file";
    const char *mode = "wb"; // Write mode for gzputc, gzputs, gzprintf

    // Prepare environment: write raw data to dummy file first if needed for seek tests later
    // For write mode, the file content doesn't strictly matter for the write operations,
    // but we create the file to ensure the path exists.
    write_dummy_file(Data, Size);

    gzFile file = gzopen(filename, mode);
    if (!file) {
        return 0;
    }

    // 1. gzputc
    // Write a character derived from the input data
    int c = Data[0];
    gzputc(file, c);

    // 2. gzputs
    // Create a null-terminated string from input data
    // We limit the length to avoid integer overflow issues mentioned in the prompt
    size_t max_str_len = Size > 1000 ? 1000 : Size;
    char *str = (char *)malloc(max_str_len + 1);
    if (str) {
        memcpy(str, Data, max_str_len);
        str[max_str_len] = '\0';
        gzputs(file, str);
        free(str);
    }

    // 3. gzerror (first call)
    int errnum;
    const char *err_msg = gzerror(file, &errnum);
    (void)err_msg; // Suppress unused warning

    // 4. gzprintf
    // Use a simple format string to avoid complex parsing issues with fuzz data
    // We use the first few bytes as an integer if possible, or just a fixed format
    int val = 0;
    if (Size >= 4) {
        val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    gzprintf(file, "Test value: %d\n", val);

    // 5. gzerror (second call)
    err_msg = gzerror(file, &errnum);
    (void)err_msg;

    // 6. gzseek
    // Seek to a position derived from input data.
    // Note: gzseek on a write-only file might behave differently or fail depending on implementation.
    // We use SEEK_SET and a safe offset.
    z_off_t offset = 0;
    if (Size >= 4) {
        offset = (z_off_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        // Ensure offset is non-negative for safety
        if (offset < 0) offset = 0;
    }
    gzseek(file, offset, SEEK_SET);

    // Cleanup
    gzclose(file);
    return 0;
}