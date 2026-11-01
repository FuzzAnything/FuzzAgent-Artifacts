// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
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

static void cleanup_file(gzFile *file) {
    if (file && *file) {
        gzclose(*file);
        *file = NULL;
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int errnum;
    const char *dummy_file = "./dummy_file";
    
    // Ensure minimum size for meaningful fuzzing
    if (Size < 2) {
        return 0;
    }

    // Step 1: Open file for writing
    file = gzopen(dummy_file, "wb");
    if (!file) {
        return 0;
    }

    // Step 2: Invoke gzputc
    // Use first byte as character, ensure it's not EOF (-1)
    int c = Data[0];
    if (c == -1) c = 0;
    gzputc(file, c);

    // Step 3: Invoke gzputs
    // Create a null-terminated string from input data
    size_t str_len = Size - 1;
    if (str_len > 0) {
        // Ensure we don't exceed int range for string length
        if (str_len > (size_t)INT_MAX) {
            str_len = INT_MAX;
        }
        // Ensure null termination by copying up to str_len and adding null
        char *str = (char *)malloc(str_len + 1);
        if (str) {
            memcpy(str, Data + 1, str_len);
            str[str_len] = '\0';
            gzputs(file, str);
            free(str);
        }
    }

    // Step 4: Invoke gzerror (first time)
    gzerror(file, &errnum);

    // Step 5: Invoke gzprintf
    // Use format string carefully to avoid crashes
    // We'll use a simple format with one integer argument from the data
    if (Size > 2) {
        int val = (int)Data[2];
        gzprintf(file, "Value: %d\n", val);
    } else {
        gzprintf(file, "No value\n");
    }

    // Step 6: Invoke gzerror (second time)
    gzerror(file, &errnum);

    // Step 7: Invoke gzseek
    // Seek to beginning if possible, or use a small offset
    z_off_t offset = 0;
    if (Size > 3) {
        offset = (z_off_t)(Data[3] & 0xFF);
    }
    gzseek(file, offset, SEEK_SET);

    // Cleanup
    cleanup_file(&file);
    
    // Remove the dummy file
    remove(dummy_file);

    return 0;
}