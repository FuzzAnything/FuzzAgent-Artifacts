// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
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

static void safe_gzclose(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare a dummy file path
    const char *filename = "./dummy_file";

    // Open the file in write mode to test the sequence
    gzFile f = gzopen(filename, "wb");
    if (f == NULL) {
        return 0;
    }

    // 1. gzputc: Write a single character from input data
    int c = (Size > 0) ? Data[0] : 0;
    gzputc(f, c);

    // 2. gzputs: Write a string from input data (ensure null termination)
    if (Size > 1) {
        // Create a temporary buffer to ensure null termination
        char *str = (char *)malloc(Size + 1);
        if (str != NULL) {
            memcpy(str, Data + 1, Size - 1);
            str[Size - 1] = '\0';
            gzputs(f, str);
            free(str);
        }
    }

    // 3. gzerror: Check for errors after writes
    int errnum = 0;
    const char *err_msg = gzerror(f, &errnum);
    (void)err_msg; // Suppress unused variable warning

    // 4. gzprintf: Format and write data
    if (Size > 2) {
        char *fmt = (char *)malloc(Size + 1);
        if (fmt != NULL) {
            memcpy(fmt, Data + 2, Size - 2);
            fmt[Size - 2] = '\0';
            // Ensure the format string is safe by limiting arguments if needed
            // For fuzzing, we pass a simple integer argument to avoid complex parsing issues
            gzprintf(f, fmt, (int)Data[0]);
            free(fmt);
        }
    }

    // 5. gzerror: Check for errors after gzprintf
    err_msg = gzerror(f, &errnum);
    (void)err_msg;

    // 6. gzseek: Attempt to seek within the file
    // Use a small offset derived from input to avoid overflow
    z_off_t offset = 0;
    if (Size > 3) {
        offset = (z_off_t)(Data[3] & 0x7F); // Keep it small and positive
    }
    gzseek(f, offset, SEEK_SET);

    // Cleanup
    safe_gzclose(f);

    return 0;
}