// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy file for reading/writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open file for reading
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (!file_read) {
        return 0;
    }

    // Test gzgets
    char buffer[1024];
    char *result = gzgets(file_read, buffer, sizeof(buffer));
    (void)result; // Use result to avoid unused variable warning

    // Test gzdirect
    int direct = gzdirect(file_read);
    (void)direct;

    // Test gzungetc - push back first character if we read something
    if (buffer[0] != '\0') {
        int unget_result = gzungetc(buffer[0], file_read);
        (void)unget_result;
    }

    // Test gzerror
    int errnum;
    const char *error_msg = gzerror(file_read, &errnum);
    (void)error_msg;

    // Close reading file
    gzclose(file_read);

    // Open file for writing
    gzFile file_write = gzopen("./dummy_file", "wb");
    if (!file_write) {
        return 0;
    }

    // Test gzprintf - use a safe format string
    int print_result = gzprintf(file_write, "Data: %.*s", (int)(Size > 100 ? 100 : Size), Data);
    (void)print_result;

    // Test gzflush with different flush modes
    int flush_result = gzflush(file_write, Z_NO_FLUSH);
    (void)flush_result;
    
    flush_result = gzflush(file_write, Z_SYNC_FLUSH);
    (void)flush_result;

    // Test gzerror on write file
    error_msg = gzerror(file_write, &errnum);
    (void)error_msg;

    // Close writing file
    gzclose(file_write);

    // Clean up
    remove("./dummy_file");

    return 0;
}