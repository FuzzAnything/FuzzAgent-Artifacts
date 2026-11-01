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
#include <stdio.h>
#include <string.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare environment: write data to dummy file if needed for reading
    // For this sequence, we primarily need write mode, but let's ensure file exists
    write_dummy_file(Data, Size);

    // Open file for writing
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) {
        return 0;
    }

    // 1. gzputc: Write a single character
    int c = Data[0];
    int ret_putc = gzputc(file, c);
    (void)ret_putc; // Suppress unused variable warning

    // 2. gzputs: Write a string
    // Ensure the string is null-terminated and safe
    size_t str_len = (Size > 1) ? (Size - 1) : 0;
    if (str_len > 0) {
        char *str = (char *)malloc(str_len + 1);
        if (str) {
            memcpy(str, Data + 1, str_len);
            str[str_len] = '\0';
            int ret_puts = gzputs(file, str);
            (void)ret_puts;
            free(str);
        }
    }

    // 3. gzerror: Check for errors
    int errnum = 0;
    const char *err_msg = gzerror(file, &errnum);
    (void)err_msg;
    (void)errnum;

    // 4. gzprintf: Format and write
    int ret_printf = gzprintf(file, "Data: %d bytes, char: %c\n", (int)Size, Data[0]);
    (void)ret_printf;

    // 5. gzerror: Check for errors again
    err_msg = gzerror(file, &errnum);
    (void)err_msg;
    (void)errnum;

    // 6. gzseek: Seek within the file
    // Use a safe offset based on input data, clamped to reasonable values
    z_off_t offset = 0;
    if (Size > 0) {
        offset = (z_off_t)Data[0] % 100; // Keep offset small and positive
    }
    z_off_t ret_seek = gzseek(file, offset, SEEK_SET);
    (void)ret_seek;

    // Cleanup: Close the file
    gzclose(file);

    return 0;
}